/*
Command to run from root directory:
make test

Or manually:
gcc -Wall -Wextra -g -o _test_kruskal tests/testKruskal.c src/graph.c src/unionFind.c src/kruskal.c -I./src
./_test_kruskal
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "kruskal.h"
#include "unionFind.h"

// Helper function to verify MST properties
int isValidMST(graph* g, mstResult* result) {
    // Check 1: Edge count should be num_nodes - 1
    if (result->edgeCount != g->num_nodes - 1) {
        printf("Error: Expected %d edges, got %d\n", g->num_nodes - 1, result->edgeCount);
        return 0;
    }
    
    // Check 2: All nodes should be connected (using union-find)
    unionFind* uf = ufCreate(g->num_nodes);
    for (int i = 0; i < result->edgeCount; i++) {
        ufUnion(uf, result->edges[i].src, result->edges[i].dest);
    }
    
    // Check if all nodes are in one connected component
    int root = -1;
    for (int i = 0; i < g->num_nodes; i++) {
        if (root == -1) {
            root = ufFindParent(uf, i);
        } else if (ufFindParent(uf, i) != root) {
            printf("Error: Graph is not fully connected\n");
            ufFree(uf);
            return 0;
        }
    }
    
    ufFree(uf);
    return 1;
}

// Helper function to calculate total cost manually
int calculateTotalCost(mstResult* result) {
    int total = 0;
    for (int i = 0; i < result->edgeCount; i++) {
        total += result->edges[i].weight;
    }
    return total;
}

// Test 1: Simple 3-node graph
void test_simple_triangle() {
    printf("Test 1: Simple 3-node triangle graph\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 2);
    addEdge(g, 0, 2, 3);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 2);  // 3 nodes - 1
    assert(result->totalCost == 3);  // 1 + 2
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 1 passed\n\n");
}

// Test 2: Single node graph
void test_single_node() {
    printf("Test 2: Single node graph\n");
    graph* g = createGraph(1);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 0);  // 1 - 1 = 0 edges
    assert(result->totalCost == 0);
    
    mstFree(result);
    graphFree(g);
    printf("Test 2 passed\n\n");
}

// Test 3: Two node graph
void test_two_nodes() {
    printf("Test 3: Two node graph\n");
    graph* g = createGraph(2);
    
    addEdge(g, 0, 1, 5);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 1);
    assert(result->totalCost == 5);
    assert(result->edges[0].weight == 5);
    
    mstFree(result);
    graphFree(g);
    printf("Test 3 passed\n\n");
}

// Test 4: Square graph (4 nodes)
void test_square_graph() {
    printf("Test 4: Square graph (4 nodes)\n");
    graph* g = createGraph(4);
    
    // Create a square with diagonal
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 2);
    addEdge(g, 2, 3, 3);
    addEdge(g, 3, 0, 4);
    addEdge(g, 0, 2, 5);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 3);  // 4 - 1 = 3
    assert(result->totalCost == 6);  // 1 + 2 + 3
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d (Expected: 6)\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 4 passed\n\n");
}

// Test 5: Complete graph K5
void test_complete_graph() {
    printf("Test 5: Complete graph K5 (5 fully connected nodes)\n");
    graph* g = createGraph(5);
    
    // Add edges with increasing weights
    int weight = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            addEdge(g, i, j, weight);
            weight++;
        }
    }
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 4);  // 5 - 1 = 4
    // Minimum should be 1+2+3+4 = 10
    assert(result->totalCost == 10);
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d (Expected: 10)\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 5 passed\n\n");
}

// Test 6: Disconnected graph (should still work for each component)
void test_disconnected_graph() {
    printf("Test 6: Disconnected graph (two separate components)\n");
    graph* g = createGraph(4);
    
    // Component 1: nodes 0, 1
    addEdge(g, 0, 1, 5);
    
    // Component 2: nodes 2, 3
    addEdge(g, 2, 3, 10);
    
    // Note: Kruskal will create an MST for each component
    // but the result will only include edges from both components
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 2);  // Both edges needed for connectivity
    assert(result->totalCost == 15);
    
    mstFree(result);
    graphFree(g);
    printf("Test 6 passed\n\n");
}

// Test 7: Graph with duplicate edge weights
void test_duplicate_weights() {
    printf("Test 7: Graph with duplicate edge weights\n");
    graph* g = createGraph(4);
    
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 5);
    addEdge(g, 2, 3, 5);
    addEdge(g, 0, 3, 5);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 3);  // 4 - 1 = 3
    assert(result->totalCost == 15);  // 5 + 5 + 5
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 7 passed\n\n");
}

// Test 8: Large weight differences
void test_large_weight_differences() {
    printf("Test 8: Large weight differences\n");
    graph* g = createGraph(4);
    
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 1000);
    addEdge(g, 2, 3, 1);
    addEdge(g, 0, 3, 999);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 3);
    // Should select: 0-1 (1), 2-3 (1), 0-3 (999) = 1001
    // NOT: 0-1 (1), 1-2 (1000), 2-3 (1) = 1002
    assert(result->totalCost == 1001);
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d (Expected: 1001)\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 8 passed\n\n");
}

// Test 9: Linear chain graph
void test_linear_chain() {
    printf("Test 9: Linear chain graph (0-1-2-3-4)\n");
    graph* g = createGraph(5);
    
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 2);
    addEdge(g, 2, 3, 3);
    addEdge(g, 3, 4, 4);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 4);  // 5 - 1 = 4
    assert(result->totalCost == 10);  // 1 + 2 + 3 + 4
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 9 passed\n\n");
}

// Test 10: Star topology (one central node connected to all others)
void test_star_topology() {
    printf("Test 10: Star topology (central node 0 connected to all)\n");
    graph* g = createGraph(5);
    
    // Connect node 0 to all others with different weights
    for (int i = 1; i < 5; i++) {
        addEdge(g, 0, i, i);
    }
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 4);
    assert(result->totalCost == 10);  // 1 + 2 + 3 + 4
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 10 passed\n\n");
}

// Test 11: Cycle graph (redundant edges everywhere)
void test_cycle_graph() {
    printf("Test 11: Cycle graph with redundant edges\n");
    graph* g = createGraph(6);
    
    // Create a hexagon
    addEdge(g, 0, 1, 10);
    addEdge(g, 1, 2, 10);
    addEdge(g, 2, 3, 10);
    addEdge(g, 3, 4, 10);
    addEdge(g, 4, 5, 10);
    addEdge(g, 5, 0, 10);
    
    // Add internal chords with lower weights
    addEdge(g, 0, 3, 1);
    addEdge(g, 1, 4, 1);
    addEdge(g, 2, 5, 1);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 5);  // 6 - 1 = 5
    // Should pick 3 chords (weight 1 each) + 2 more edges
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 11 passed\n\n");
}

// Test 12: mstPrint and mstFree functions
void test_mst_print_and_free() {
    printf("Test 12: Testing mstPrint and mstFree functions\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 3);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    printf("  Output of mstPrint:\n  ");
    mstPrint(result);
    
    mstFree(result);
    
    // Test mstFree with NULL (should not crash)
    mstFree(NULL);
    
    graphFree(g);
    printf("Test 12 passed\n\n");
}

// Test 13: Edge selection from multiple options (should select minimum)
void test_multiple_edges_same_nodes() {
    printf("Test 13: Edge selection with multiple options\n");
    graph* g = createGraph(3);
    
    // Create a triangle with different weights
    addEdge(g, 0, 1, 100);
    addEdge(g, 1, 2, 50);
    addEdge(g, 0, 2, 75);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 2);
    // Should select 1-2 (50) and 0-2 (75) = 125, NOT 0-1 (100) which forms a cycle
    assert(result->totalCost == 125);
    
    mstFree(result);
    graphFree(g);
    printf("Test 13 passed\n\n");
}

// Test 14: Edge case with weight = 0
void test_zero_weight_edges() {
    printf("Test 14: Edges with zero weight\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 0);
    addEdge(g, 1, 2, 0);
    addEdge(g, 0, 2, 10);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 2);
    assert(result->totalCost == 0);  // 0 + 0
    assert(isValidMST(g, result));
    
    mstFree(result);
    graphFree(g);
    printf("Test 14 passed\n\n");
}

// Test 15: Grid graph (3x3 mesh)
void test_grid_graph() {
    printf("Test 15: 3x3 grid graph\n");
    graph* g = createGraph(9);
    
    // Create 3x3 grid
    // 0 - 1 - 2
    // |   |   |
    // 3 - 4 - 5
    // |   |   |
    // 6 - 7 - 8
    
    // Horizontal edges
    addEdge(g, 0, 1, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 3, 4, 1);
    addEdge(g, 4, 5, 1);
    addEdge(g, 6, 7, 1);
    addEdge(g, 7, 8, 1);
    
    // Vertical edges
    addEdge(g, 0, 3, 1);
    addEdge(g, 3, 6, 1);
    addEdge(g, 1, 4, 1);
    addEdge(g, 4, 7, 1);
    addEdge(g, 2, 5, 1);
    addEdge(g, 5, 8, 1);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 8);  // 9 - 1 = 8
    assert(result->totalCost == 8);  // All weights are 1
    assert(isValidMST(g, result));
    
    printf("  MST Cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 15 passed\n\n");
}

// Test 16: Verify total cost calculation consistency
void test_cost_consistency() {
    printf("Test 16: Verify total cost calculation consistency\n");
    graph* g = createGraph(5);
    
    addEdge(g, 0, 1, 7);
    addEdge(g, 0, 2, 5);
    addEdge(g, 1, 3, 8);
    addEdge(g, 1, 4, 9);
    addEdge(g, 2, 3, 15);
    addEdge(g, 2, 4, 6);
    addEdge(g, 3, 4, 11);
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    int manualCost = calculateTotalCost(result);
    assert(manualCost == result->totalCost);
    assert(isValidMST(g, result));
    
    printf("  Reported cost: %d, Calculated cost: %d\n", result->totalCost, manualCost);
    mstFree(result);
    graphFree(g);
    printf("Test 16 passed\n\n");
}

// Test 17: Large graph stress test
void test_large_graph() {
    printf("Test 17: Large graph (100 nodes, linear chain)\n");
    graph* g = createGraph(100);
    
    // Create linear chain
    for (int i = 0; i < 99; i++) {
        addEdge(g, i, i + 1, 1);
    }
    
    mstResult* result = kruskal(g);
    
    assert(result != NULL);
    assert(result->edgeCount == 99);  // 100 - 1
    assert(result->totalCost == 99);  // 99 edges with weight 1
    assert(isValidMST(g, result));
    
    printf("  MST with 100 nodes, cost: %d\n", result->totalCost);
    mstFree(result);
    graphFree(g);
    printf("Test 17 passed\n\n");
}

int main() {
    printf("=== Running Kruskal's Algorithm Tests ===\n\n");
    
    test_simple_triangle();
    test_single_node();
    test_two_nodes();
    test_square_graph();
    test_complete_graph();
    test_disconnected_graph();
    test_duplicate_weights();
    test_large_weight_differences();
    test_linear_chain();
    test_star_topology();
    test_cycle_graph();
    test_mst_print_and_free();
    test_multiple_edges_same_nodes();
    test_zero_weight_edges();
    test_grid_graph();
    test_cost_consistency();
    test_large_graph();
    
    printf("=== All Kruskal tests passed! ===\n");
    
    return 0;
}
