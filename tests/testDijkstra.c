/*
Command to run (from project root):
gcc -Wall -Wextra -g -o _test_dijkstra tests/testDijkstra.c src/dijkstra.c src/graph.c src/minHeap.c -I./src
./_test_dijkstra
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "graph.h"

// Helper function to verify path correctness
static int verify_path(pathResult* result, int* expected_path, int expected_length, int expected_cost) {
    if (!result) return 0;
    if (result->pathLength != expected_length) return 0;
    if (result->totalCost != expected_cost) return 0;
    
    for (int i = 0; i < expected_length; i++) {
        if (result->path[i] != expected_path[i]) {
            return 0;
        }
    }
    return 1;
}

// Test 1: Simple linear graph 0->1->2->3
static void test_linear_path(void) {
    printf("Test 1: Simple linear path (0->1->2->3)\n");
    
    graph* g = createGraph(4);
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 2, 3, 1);
    
    pathResult* result = dijkstraShortestPath(g, 0, 3);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 3);
    assert(result->pathLength == 4);
    
    int expected[] = {0, 1, 2, 3};
    assert(verify_path(result, expected, 4, 3));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 1 passed\n\n");
}

// Test 2: Diamond graph with multiple paths
static void test_diamond_graph(void) {
    printf("Test 2: Diamond graph (multiple paths)\n");
    
    graph* g = createGraph(4);
    // Paths: 0->1->3 (cost 2) and 0->2->3 (cost 5)
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 0, 2, 2);
    addEdge(g, 2, 3, 3);
    
    pathResult* result = dijkstraShortestPath(g, 0, 3);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 2);  // Should take path 0->1->3
    assert(result->pathLength == 3);
    
    int expected[] = {0, 1, 3};
    assert(verify_path(result, expected, 3, 2));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 2 passed\n\n");
}

// Test 3: Source equals destination
static void test_source_equals_destination(void) {
    printf("Test 3: Source equals destination\n");
    
    graph* g = createGraph(3);
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 3);
    
    pathResult* result = dijkstraShortestPath(g, 1, 1);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 0);
    assert(result->pathLength == 1);
    assert(result->path[0] == 1);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 3 passed\n\n");
}

// Test 4: Unreachable destination
static void test_unreachable_destination(void) {
    printf("Test 4: Unreachable destination (disconnected graph)\n");
    
    graph* g = createGraph(4);
    // Component 1: 0->1
    addEdge(g, 0, 1, 5);
    // Component 2: 2->3 (disconnected from 0-1)
    addEdge(g, 2, 3, 3);
    
    pathResult* result = dijkstraShortestPath(g, 0, 3);
    assert(result != NULL);
    assert(result->reachable == 0);
    assert(result->totalCost == -1);
    assert(result->pathLength == 0);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 4 passed\n\n");
}

// Test 5: Single node graph
static void test_single_node(void) {
    printf("Test 5: Single node graph\n");
    
    graph* g = createGraph(1);
    
    pathResult* result = dijkstraShortestPath(g, 0, 0);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 0);
    assert(result->pathLength == 1);
    assert(result->path[0] == 0);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 5 passed\n\n");
}

// Test 6: Two node graph
static void test_two_nodes(void) {
    printf("Test 6: Two node graph\n");
    
    graph* g = createGraph(2);
    addEdge(g, 0, 1, 10);
    
    pathResult* result = dijkstraShortestPath(g, 0, 1);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 10);
    assert(result->pathLength == 2);
    assert(result->path[0] == 0);
    assert(result->path[1] == 1);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 6 passed\n\n");
}

// Test 7: Graph with multiple paths
static void test_graph_with_multiple_paths(void) {
    printf("Test 7: Graph with multiple paths\n");
    
    graph* g = createGraph(4);
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 2);
    addEdge(g, 2, 3, 5);
    addEdge(g, 1, 3, 3);
    
    pathResult* result = dijkstraShortestPath(g, 0, 3);
    assert(result != NULL);
    assert(result->reachable == 1);
    // Path 0->1->3: 4+3 = 7
    // Path 0->2->3: 2+5 = 7
    // Both are equal, so cost should be 7
    assert(result->totalCost == 7);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 7 passed\n\n");
}

// Test 8: Larger network (6 nodes)
static void test_larger_network(void) {
    printf("Test 8: Larger network (6 nodes)\n");
    
    graph* g = createGraph(6);
    // Create a more complex network
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 2);
    addEdge(g, 1, 2, 1);
    addEdge(g, 1, 3, 5);
    addEdge(g, 2, 3, 8);
    addEdge(g, 2, 4, 10);
    addEdge(g, 3, 4, 2);
    addEdge(g, 3, 5, 6);
    addEdge(g, 4, 5, 3);
    
    pathResult* result = dijkstraShortestPath(g, 0, 5);
    assert(result != NULL);
    assert(result->reachable == 1);
    // Shortest path: 0->2->1->3->4->5 with cost 2+1+5+2+3 = 13
    assert(result->totalCost == 13);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 8 passed\n\n");
}

// Test 9: Null graph handling
static void test_null_graph(void) {
    printf("Test 9: Null graph handling\n");
    
    pathResult* result = dijkstraShortestPath(NULL, 0, 1);
    assert(result == NULL);
    
    printf("Test 9 passed\n\n");
}

// Test 10: All nodes connected with equal weights
static void test_equal_weight_graph(void) {
    printf("Test 10: Graph with equal weight edges\n");
    
    graph* g = createGraph(4);
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 2, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 2, 3, 1);
    
    pathResult* result = dijkstraShortestPath(g, 0, 3);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 2);  // Any 2-hop path has cost 2
    assert(result->pathLength == 3);  // 3 nodes in path (0, intermediate, 3)
    
    // Path should start with 0 and end with 3
    assert(result->path[0] == 0);
    assert(result->path[2] == 3);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 10 passed\n\n");
}

// Test 11: High cost edge should be avoided
static void test_high_cost_edge_avoidance(void) {
    printf("Test 11: High cost edge avoidance\n");
    
    graph* g = createGraph(3);
    addEdge(g, 0, 2, 1000);  // Direct but expensive path
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 1);     // Cheaper indirect path
    
    pathResult* result = dijkstraShortestPath(g, 0, 2);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 2);  // Should take 0->1->2, not 0->2
    assert(result->pathLength == 3);
    
    int expected[] = {0, 1, 2};
    assert(verify_path(result, expected, 3, 2));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 11 passed\n\n");
}

// Test 12: Starting from non-zero node
static void test_non_zero_source(void) {
    printf("Test 12: Starting from non-zero source node\n");
    
    graph* g = createGraph(4);
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 3);
    addEdge(g, 2, 3, 2);
    
    pathResult* result = dijkstraShortestPath(g, 1, 3);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 5);  // 1->2->3: 3+2 = 5
    assert(result->pathLength == 3);
    
    int expected[] = {1, 2, 3};
    assert(verify_path(result, expected, 3, 5));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 12 passed\n\n");
}

// Test 13: Direct unreachable destination (adjacent node not connected)
static void test_unreachable_adjacent_node(void) {
    printf("Test 13: Unreachable adjacent node\n");
    
    graph* g = createGraph(3);
    addEdge(g, 0, 1, 5);
    // Node 2 has no incoming edges from 0 or 1
    
    pathResult* result = dijkstraShortestPath(g, 0, 2);
    assert(result != NULL);
    assert(result->reachable == 0);
    assert(result->totalCost == -1);
    assert(result->pathLength == 0);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 13 passed\n\n");
}

// Test 14: Multiple paths with different lengths and costs
static void test_multiple_paths_different_costs(void) {
    printf("Test 14: Multiple paths with different costs\n");
    
    graph* g = createGraph(5);
    // Path 1: 0->1->4 (cost 1+1 = 2, length 3)
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 4, 1);
    // Path 2: 0->2->3->4 (cost 10+5+5 = 20, length 4)
    addEdge(g, 0, 2, 10);
    addEdge(g, 2, 3, 5);
    addEdge(g, 3, 4, 5);
    
    pathResult* result = dijkstraShortestPath(g, 0, 4);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 2);  // Should take Path 1
    assert(result->pathLength == 3);
    
    int expected[] = {0, 1, 4};
    assert(verify_path(result, expected, 3, 2));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 14 passed\n\n");
}

// Test 15: Pathresult printing test
static void test_pathresult_print(void) {
    printf("Test 15: Path result printing\n");
    
    graph* g = createGraph(3);
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 3);
    
    pathResult* result = dijkstraShortestPath(g, 0, 2);
    assert(result != NULL);
    
    printf("Testing pathResultPrint output:\n");
    pathResultPrint(result);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 15 passed\n\n");
}

// Test 16: Pathresult print with unreachable node
static void test_pathresult_print_unreachable(void) {
    printf("Test 16: Path result printing (unreachable)\n");
    
    graph* g = createGraph(3);
    addEdge(g, 0, 1, 5);
    
    pathResult* result = dijkstraShortestPath(g, 0, 2);
    assert(result != NULL);
    
    printf("Testing pathResultPrint output for unreachable:\n");
    pathResultPrint(result);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 16 passed\n\n");
}

// Test 17: Pathresult null safety
static void test_pathresult_null_print(void) {
    printf("Test 17: Path result null pointer safety\n");
    
    pathResultPrint(NULL);
    
    printf("Test 17 passed\n\n");
}

// Test 18: Pathresult null free safety
static void test_pathresult_null_free(void) {
    printf("Test 18: Path result null free safety\n");
    
    pathResultFree(NULL);
    
    printf("Test 18 passed\n\n");
}

// Test 19: Complex network with disconnected component
static void test_network_topology(void) {
    printf("Test 19: Complex network with disconnected component\n");
    
    graph* g = createGraph(10);
    
    // Connected component 1: 0-1-2-3-4-5
    addEdge(g, 0, 1, 7);
    addEdge(g, 0, 2, 9);
    addEdge(g, 0, 5, 14);
    addEdge(g, 1, 2, 10);
    addEdge(g, 1, 3, 15);
    addEdge(g, 2, 3, 11);
    addEdge(g, 2, 5, 2);
    addEdge(g, 3, 4, 6);
    addEdge(g, 4, 5, 9);
    
    // Disconnected component 2: 6-7-8-9 (not reachable from 0)
    addEdge(g, 6, 7, 14);
    addEdge(g, 7, 8, 7);
    addEdge(g, 8, 9, 10);
    
    pathResult* result = dijkstraShortestPath(g, 0, 9);
    assert(result != NULL);
    assert(result->reachable == 0);  // 9 is in disconnected component
    assert(result->totalCost == -1);
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 19 passed\n\n");
}

// Test 20: Balanced tree structure
static void test_balanced_tree(void) {
    printf("Test 20: Balanced tree structure\n");
    
    graph* g = createGraph(7);
    // Create a balanced binary tree
    //   Root: 0
    //   Level 1: 1, 2
    //   Level 2: 3, 4, 5, 6
    
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 2, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 1, 4, 1);
    addEdge(g, 2, 5, 1);
    addEdge(g, 2, 6, 1);
    
    pathResult* result = dijkstraShortestPath(g, 0, 6);
    assert(result != NULL);
    assert(result->reachable == 1);
    assert(result->totalCost == 2);  // 0->2->6
    assert(result->pathLength == 3);
    
    int expected[] = {0, 2, 6};
    assert(verify_path(result, expected, 3, 2));
    
    pathResultFree(result);
    graphFree(g);
    printf("Test 20 passed\n\n");
}

int main(void) {
    printf("===== Running Dijkstra Algorithm Tests =====\n\n");
    
     test_linear_path();
     test_diamond_graph();
     test_source_equals_destination();
     test_unreachable_destination();
     test_single_node();
     test_two_nodes();
     test_graph_with_multiple_paths();
     test_larger_network();
     test_null_graph();
     test_equal_weight_graph();
     test_high_cost_edge_avoidance();
     test_non_zero_source();
     test_unreachable_adjacent_node();
     test_multiple_paths_different_costs();
     test_pathresult_print();
     test_pathresult_print_unreachable();
     test_pathresult_null_print();
     test_pathresult_null_free();
     test_network_topology();
     test_balanced_tree();
    
    printf("===== All Dijkstra tests passed! =====\n");
    return 0;
}
