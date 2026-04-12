/*
Command to run : 
gcc -o testGraph testGraph.c ../src/graph.c -I./../src


*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

void test_createGraph() {
    printf("Test 1: Creating an empty graph with 5 nodes\n");
    graph* g = createGraph(5);
    
    assert(g != NULL);
    assert(g->num_nodes == 5);
    assert(g->num_edges == 0);
    
    for (int i = 0; i < 5; i++) {
        assert(g->adjList[i].head == NULL);
        assert(g->adjList[i].tail == NULL);
    }
    
    graphFree(g);
    printf("Test 1 passed\n\n");
}

void test_addSingleEdge() {
    printf("Test 2: Adding a single edge\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 10);
    
    //Check forward edge
    assert(g->adjList[0].head != NULL);
    assert(g->adjList[0].head->dest == 1);
    assert(g->adjList[0].head->weight == 10);
    assert(g->adjList[0].head->next == NULL);
    assert(g->adjList[0].tail == g->adjList[0].head);
    
    //Check reverse edge (undirected)
    assert(g->adjList[1].head != NULL);
    assert(g->adjList[1].head->dest == 0);
    assert(g->adjList[1].head->weight == 10);
    assert(g->adjList[1].head->next == NULL);
    
    //num_edges should be 1 (not 2)
    assert(g->num_edges == 1);

    edge* edges = getAllEdges(g, &(g->num_edges));
    assert(edges[0].src == 0);
    assert(edges[0].dest == 1);
    assert(edges[0].weight == 10);
    free(edges);

    graphFree(g);
    printf("Test 2 passed\n\n");
}

void test_addMultipleEdges() {
    printf("Test 3: Adding multiple edges from one node\n");
    graph* g = createGraph(4);
    
    addEdge(g, 0, 1, 5);
    addEdge(g, 0, 2, 10);
    addEdge(g, 0, 3, 15);
    
    //Check that edges are in the correct order (added to tail)
    adjNode* curr = g->adjList[0].head;
    assert(curr->dest == 1 && curr->weight == 5);
    curr = curr->next;
    assert(curr->dest == 2 && curr->weight == 10);
    curr = curr->next;
    assert(curr->dest == 3 && curr->weight == 15);
    assert(curr->next == NULL);
    
    //Check tail pointer
    assert(g->adjList[0].tail->dest == 3);
    
    //num_edges should be 3
    assert(g->num_edges == 3);
    
    graphFree(g);
    printf("Test 3 passed\n\n");
}

void test_getAllEdges_simple() {
    printf("Test 4: getAllEdges with simple graph\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 5);
    addEdge(g, 1, 2, 10);
    
    int count = 0;
    edge* edges = getAllEdges(g, &count);
    
    //Should return 2 edges (not 4, since undirected)
    assert(count == 2);
    
    //Check edges are returned correctly
    assert(edges[0].src == 0 && edges[0].dest == 1 && edges[0].weight == 5);
    assert(edges[1].src == 1 && edges[1].dest == 2 && edges[1].weight == 10);
    
    free(edges);
    graphFree(g);
    printf("Test 4 passed\n\n");
}

void test_getAllEdges_complex() {
    printf("Test 5: getAllEdges with complex graph\n");
    graph* g = createGraph(4);
    
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 2, 2);
    addEdge(g, 1, 3, 3);
    addEdge(g, 2, 3, 4);
    
    int count = 0;
    edge* edges = getAllEdges(g, &count);
    
    //Should return 4 unique edges
    assert(count == 4);
    
    //Verify all edges
    printf("Returned edges:\n");
    for (int i = 0; i < count; i++) {
        printf("  Edge %d: %d -> %d (weight: %d)\n", i, edges[i].src, edges[i].dest, edges[i].weight);
        assert(edges[i].src < edges[i].dest);  //src should always be less than dest
    }
    
    free(edges);
    graphFree(g);
    printf("Test 5 passed\n\n");
}

void test_graphPrint() {
    printf("Test 6: Print graph structure\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 1, 5);
    addEdge(g, 0, 2, 10);
    addEdge(g, 1, 2, 15);
    
    printf("Graph structure:\n");
    graphPrint(g);
    
    graphFree(g);
    printf("Test 6 passed\n\n");
}

void test_emptyGraph() {
    printf("Test 7: getAllEdges on empty graph\n");
    graph* g = createGraph(5);
    
    int count = 0;
    edge* edges = getAllEdges(g, &count);
    
    assert(count == 0);
    assert(edges != NULL);  //Should still allocate memory (0 size)
    
    free(edges);
    graphFree(g);
    printf("Test 7 passed\n\n");
}

void test_selfLoop() {
    printf("Test 8: Self loop edge\n");
    graph* g = createGraph(3);
    
    addEdge(g, 0, 0, 5);  //Self loop
    
    assert(g->num_edges == 1);
    assert(g->adjList[0].head->dest == 0);
    assert(g->adjList[0].head->weight == 5);
    
    //Check getAllEdges with self loop
    int count = 0;
    edge* edges = getAllEdges(g, &count);
    
    //Self loop should NOT be included (since 0 < 0 is false)
    assert(count == 0);
    
    free(edges);
    graphFree(g);
    printf("Test 8 passed\n\n");
}

void test_memory_integrity() {
    printf("Test 9: Memory integrity check\n");
    graph* g = createGraph(2);
    
    for (int i = 0; i < 100; i++) {
        addEdge(g, 0, 1, i);
    }
    
    int count = 0;
    edge* edges = getAllEdges(g, &count);
    
    //Should only have 1 edge (0->1), but added 100 times
    //This reveals if addEdge is working correctly
    printf("Added edge (0->1) 100 times, got %d edges\n", count);
    assert(count == 1);
    
    free(edges);
    graphFree(g);
    printf("Test 9 passed\n\n");
}

int main() {
    printf("=== Running Graph Tests ===\n\n");
    
    test_createGraph();
    test_addSingleEdge();
    test_addMultipleEdges();
    test_getAllEdges_simple();
    test_getAllEdges_complex();
    test_graphPrint();
    test_emptyGraph();
    test_selfLoop();
    test_memory_integrity();
    
    printf("=== All tests passed! ===\n");
    
    return 0;
}
