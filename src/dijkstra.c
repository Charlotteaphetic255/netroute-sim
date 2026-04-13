#include "dijkstra.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "minHeap.h"

#define INF INT_MAX

pathResult* dijkstraShortestPath(graph* g, int src, int dest) {
    if (!g) {
        return NULL;
    }

    int n = g->num_nodes;
    int* dist = (int*)malloc(sizeof(int) * n);
    int* visited = (int*)malloc(sizeof(int) * n);
    int* parent = (int*)malloc(sizeof(int) * n);
    pathResult* result = (pathResult*)malloc(sizeof(pathResult));
    minHeap* heap = minHeapCreate(n);

    if (!dist || !visited || !parent || !result || !heap) {
        free(dist);
        free(visited);
        free(parent);
        free(result);
        minHeapFree(heap);
        return NULL;
    }

    result->path = (int*)malloc(sizeof(int) * n);
    if (!result->path) {
        free(dist);
        free(visited);
        free(parent);
        free(result);
        minHeapFree(heap);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;
    minHeapInsert(heap, src, 0);

    while (!minHeapIsEmpty(heap)) {
        heapNode crr = minHeapExtractMin(heap);
        int u = crr.node;

        if (u == -1 || visited[u]) {
            continue;
        }

        if (dist[u] == INF) {
            break;
        }

        visited[u] = 1;
        if (u == dest) {
            break;
        }

        adjNode* temp = g->adjList[u].head;
        while (temp) {
            int v = temp->dest;
            int newDist = dist[u] + temp->weight;

            if (!visited[v] && newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                if (minHeapContains(heap, v)) {
                    minHeapDecreaseKey(heap, v, newDist);
                } else {
                    minHeapInsert(heap, v, newDist);
                }
            }
            temp = temp->next;
        }
    }

    if (dist[dest] == INF) {
        result->reachable = 0;
        result->pathLength = 0;
        result->totalCost = -1;
    } else {
        result->reachable = 1;
        result->totalCost = dist[dest];

        //build the path by backtracking from dest to src using the parent array
        int curr = dest;
        int pathIdx = 0;
        while (curr != -1) {
            result->path[pathIdx++] = curr;
            curr = parent[curr];
        }
        //reverse the path to get the correct order from src to dest
        for (int i = 0; i < pathIdx / 2; i++) {
            int temp = result->path[i];
            result->path[i] = result->path[pathIdx - 1 - i];
            result->path[pathIdx - 1 - i] = temp;
        }

        result->pathLength = pathIdx;
    }
    //free the temporary arrays and the min-heap
    free(dist);
    free(visited);
    free(parent);
    minHeapFree(heap);

    return result;
}

void pathResultPrint(pathResult* result) {
    if (result == NULL) {
        printf("Error: pathResult is NULL\n");
        return;
    }
    
    if (result->reachable == 0) {
        printf("Destination is not reachable\n");
        return;
    }
    
    printf("Path: ");
    for (int i = 0; i < result->pathLength; i++) {
        printf("%d", result->path[i]);
        if (i < result->pathLength - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("Total Cost: %d\n", result->totalCost);
}

void pathResultFree(pathResult* result) {
    if (result == NULL) {
        return;
    }
    
    if (result->path != NULL) {
        free(result->path);
    }
    
    free(result);
}
