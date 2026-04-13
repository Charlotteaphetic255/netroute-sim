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

#if defined(__EMSCRIPTEN__) && defined(__has_include)
#if __has_include(<emscripten/emscripten.h>)
#include <emscripten/emscripten.h>
#endif
#endif

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))
#endif

#include "graph.h"
#include "kruskal.h"
#include "prim.h"
#include "dijkstra.h"
#include "simulator.h"
#include "loader.h"

#ifdef __EMSCRIPTEN__
static graph* wasmGraph = NULL;
static int wasmLastMstCost = -1;

EMSCRIPTEN_KEEPALIVE
int wasm_load_graph(int* edge_data, int edge_count, int node_count) {
    if (!edge_data || edge_count < 0 || node_count <= 0) {
        return 0;
    }

    if (wasmGraph) {
        graphFree(wasmGraph);
        wasmGraph = NULL;
    }

    wasmGraph = createGraph(node_count);
    if (!wasmGraph) {
        return 0;
    }

    for (int i = 0; i < edge_count; i++) {
        int baseIndex = i * 3;
        int src = edge_data[baseIndex];
        int dest = edge_data[baseIndex + 1];
        int weight = edge_data[baseIndex + 2];

        if (src < 0 || src >= node_count || dest < 0 || dest >= node_count) {
            continue;
        }

        addEdge(wasmGraph, src, dest, weight);
    }

    wasmLastMstCost = -1;
    return 1;
}

EMSCRIPTEN_KEEPALIVE
int* wasm_run_kruskal() {
    if (!wasmGraph) {
        return NULL;
    }

    mstResult* result = kruskal(wasmGraph);
    if (!result) {
        return NULL;
    }

    int outputSize = 1 + (result->edgeCount * 3);
    int* output = (int*)malloc(sizeof(int) * outputSize);
    if (!output) {
        mstFree(result);
        return NULL;
    }

    output[0] = result->edgeCount;
    for (int i = 0; i < result->edgeCount; i++) {
        int baseIndex = 1 + (i * 3);
        output[baseIndex] = result->edges[i].src;
        output[baseIndex + 1] = result->edges[i].dest;
        output[baseIndex + 2] = result->edges[i].weight;
    }

    wasmLastMstCost = result->totalCost;
    mstFree(result);
    return output;
}

EMSCRIPTEN_KEEPALIVE
int* wasm_run_prim() {
    if (!wasmGraph) {
        return NULL;
    }

    mstResult* result = primMST(wasmGraph);
    if (!result) {
        return NULL;
    }

    int outputSize = 1 + (result->edgeCount * 3);
    int* output = (int*)malloc(sizeof(int) * outputSize);
    if (!output) {
        mstFree(result);
        return NULL;
    }

    output[0] = result->edgeCount;
    for (int i = 0; i < result->edgeCount; i++) {
        int baseIndex = 1 + (i * 3);
        output[baseIndex] = result->edges[i].src;
        output[baseIndex + 1] = result->edges[i].dest;
        output[baseIndex + 2] = result->edges[i].weight;
    }

    wasmLastMstCost = result->totalCost;
    mstFree(result);
    return output;
}

EMSCRIPTEN_KEEPALIVE
int* wasm_run_dijkstra(int src, int dest) {
    if (!wasmGraph) {
        return NULL;
    }

    if (src < 0 || src >= wasmGraph->num_nodes || dest < 0 || dest >= wasmGraph->num_nodes) {
        return NULL;
    }

    pathResult* result = dijkstraShortestPath(wasmGraph, src, dest);
    if (!result) {
        return NULL;
    }

    int outputSize = 1;
    if (result->reachable) {
        outputSize += result->pathLength;
    }

    int* output = (int*)malloc(sizeof(int) * outputSize);
    if (!output) {
        pathResultFree(result);
        return NULL;
    }

    if (!result->reachable) {
        output[0] = 0;
        pathResultFree(result);
        return output;
    }

    output[0] = result->pathLength;
    for (int i = 0; i < result->pathLength; i++) {
        output[i + 1] = result->path[i];
    }

    pathResultFree(result);
    return output;
}

EMSCRIPTEN_KEEPALIVE
int wasm_get_mst_cost() {
    return wasmLastMstCost;
}

EMSCRIPTEN_KEEPALIVE
void wasm_free_result(int* ptr) {
    if (ptr) {
        free(ptr);
    }
}
#endif

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
