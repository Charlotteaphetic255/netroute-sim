/*
mingw32-make 

OR 

Run from root:
gcc -Wall -Wextra -g -o netroute-sim src/graph.c src/unionFind.c src/minHeap.c src/kruskal.c src/prim.c src/dijkstra.c src/simulator.c src/loader.c src/main.c
./netroute-sim topologies/small_network.txt --mst kruskal
./netroute-sim topologies/small_network.txt --mst prim
./netroute-sim topologies/small_network.txt --compare
./netroute-sim topologies/small_network.txt --path 0 5
./netroute-sim topologies/small_network.txt --simulate 0 5

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "kruskal.h"
#include "prim.h"
#include "dijkstra.h"
#include "simulator.h"
#include "loader.h"

static void printUsage(const char* programName) {
    printf("Usage: %s <topology_file> [options]\n", programName);
    printf("Options:\n");
    printf("  --mst kruskal     Run Kruskal and print MST\n");
    printf("  --mst prim        Run Prim and print MST\n");
    printf("  --path src dest   Find shortest path from src to dest\n");
    printf("  --compare         Run both MST algorithms and compare\n");
    printf("  --simulate src dest  Simulate packet delivery\n");
}

static int parseNodeIndex(const char* text, int* value) {
    char* endPtr = NULL;
    long parsed = strtol(text, &endPtr, 10);
    if (endPtr == text || *endPtr != '\0') {
        return 0;
    }
    if (parsed < 0 || parsed > 2147483647L) {
        return 0;
    }
    *value = (int)parsed;
    return 1;
}

static int runMst(graph* g, const char* algorithmName) {
    mstResult* result = NULL;

    if (strcmp(algorithmName, "kruskal") == 0) {
        printf("Running Kruskal MST\n\n");
        result = kruskal(g);
    } else if (strcmp(algorithmName, "prim") == 0) {
        printf("Running Prim MST\n\n");
        result = primMST(g);
    } else {
        fprintf(stderr, "Error: Unknown MST algorithm '%s'\n", algorithmName);
        return 1;
    }

    if (!result) {
        fprintf(stderr, "Error: MST algorithm failed\n");
        return 1;
    }

    mstPrint(result);
    mstFree(result);
    return 0;
}

static int runPath(graph* g, const char* srcText, const char* destText) {
    int src = 0;
    int dest = 0;

    if (!parseNodeIndex(srcText, &src) || !parseNodeIndex(destText, &dest)) {
        fprintf(stderr, "Error: src and dest must be non negative integers\n");
        return 1;
    }

    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "Error: src and dest must be between 0 and %d\n", g->num_nodes - 1);
        return 1;
    }

    pathResult* result = dijkstraShortestPath(g, src, dest);
    if (!result) {
        fprintf(stderr, "Error: shortest path computation failed\n");
        return 1;
    }

    printf("Running shortest path from %d to %d\n\n", src, dest);
    pathResultPrint(result);
    pathResultFree(result);
    return 0;
}

static int runCompare(graph* g) {
    mstResult* kruskalResult = kruskal(g);
    mstResult* primResult = primMST(g);

    if (!kruskalResult || !primResult) {
        fprintf(stderr, "Error: could not run MST comparison\n");
        mstFree(kruskalResult);
        mstFree(primResult);
        return 1;
    }

    printf("MST Comparison\n\n");
    printf("| algorithm | edge_count | total_cost |\n");
    printf("| kruskal   | %10d | %10d |\n", kruskalResult->edgeCount, kruskalResult->totalCost);
    printf("| prim      | %10d | %10d |\n", primResult->edgeCount, primResult->totalCost);

    if (kruskalResult->totalCost == primResult->totalCost) {
        printf("Result: both algorithms produced the same MST cost\n");
    } else {
        printf("Result: MST costs are different\n");
    }

    mstFree(kruskalResult);
    mstFree(primResult);
    return 0;
}

static int runSimulate(graph* g, const char* srcText, const char* destText) {
    int src = 0;
    int dest = 0;

    if (!parseNodeIndex(srcText, &src) || !parseNodeIndex(destText, &dest)) {
        fprintf(stderr, "Error: src and dest must be non negative integers\n");
        return 1;
    }

    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "Error: src and dest must be between 0 and %d\n", g->num_nodes - 1);
        return 1;
    }

    simResult* result = simulatePacket(g, src, dest);
    if (!result) {
        fprintf(stderr, "Error: simulation failed\n");
        return 1;
    }

    simResultFree(result);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    const char* topologyFile = argv[1];
    graph* g = loadGraphFromFile(topologyFile);
    if (!g) {
        fprintf(stderr, "Error: failed to load topology file '%s'\n", topologyFile);
        return 1;
    }

    int exitCode = 0;

    if (strcmp(argv[2], "--mst") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Error: --mst requires one argument: kruskal or prim\n");
            printUsage(argv[0]);
            exitCode = 1;
        } else {
            exitCode = runMst(g, argv[3]);
        }
    } else if (strcmp(argv[2], "--path") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Error: --path requires two arguments: src dest\n");
            printUsage(argv[0]);
            exitCode = 1;
        } else {
            exitCode = runPath(g, argv[3], argv[4]);
        }
    } else if (strcmp(argv[2], "--compare") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: --compare does not take extra arguments\n");
            printUsage(argv[0]);
            exitCode = 1;
        } else {
            exitCode = runCompare(g);
        }
    } else if (strcmp(argv[2], "--simulate") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Error: --simulate requires two arguments: src dest\n");
            printUsage(argv[0]);
            exitCode = 1;
        } else {
            exitCode = runSimulate(g, argv[3], argv[4]);
        }
    } else {
        fprintf(stderr, "Error: unknown option '%s'\n", argv[2]);
        printUsage(argv[0]);
        exitCode = 1;
    }

    graphFree(g);
    return exitCode;
}
