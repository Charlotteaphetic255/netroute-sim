/*
gcc -Wall -Wextra -g -o _testSimulator tests/testSimulator.c src/graph.c src/minHeap.c src/dijkstra.c src/loader.c src/simulator.c; 
./_testSimulator 2>&1
*/

#include <stdio.h>
#include <stdlib.h>
#include "../src/simulator.h"
#include "../src/loader.h"

int main() {
    /* Load the small_network topology */
    graph* g = loadGraphFromFile("topologies/small_network.txt");
    if (!g) {
        fprintf(stderr, "Failed to load graph from file\n");
        return 1;
    }

    printf("Graph loaded successfully: %d nodes, %d edges\n\n", g->num_nodes, g->num_edges);

    /* Test 1: Simulation from router 0 to router 5 */
    printf("================== TEST 1: Route 0 -> 5 ==================\n");
    simResult* result1 = simulatePacket(g, 0, 5);
    if (!result1) {
        fprintf(stderr, "Simulation failed\n");
        graphFree(g);
        return 1;
    }
    simResultPrint(result1);
    simResultFree(result1);

    /* Test 2: Simulation from router 0 to router 4 */
    printf("\n================== TEST 2: Route 0 -> 4 ==================\n");
    simResult* result2 = simulatePacket(g, 0, 4);
    if (!result2) {
        fprintf(stderr, "Simulation failed\n");
        graphFree(g);
        return 1;
    }
    simResultPrint(result2);
    simResultFree(result2);

    /* Test 3: Simulation from router 1 to router 5 */
    printf("\n================== TEST 3: Route 1 -> 5 ==================\n");
    simResult* result3 = simulatePacket(g, 1, 5);
    if (!result3) {
        fprintf(stderr, "Simulation failed\n");
        graphFree(g);
        return 1;
    }
    simResultPrint(result3);
    simResultFree(result3);

    /* Clean up */
    graphFree(g);

    return 0;
}
