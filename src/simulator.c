/*
    simulator.c
    netroute-sim - packet transmission simulation using Dijkstra's shortest path
*/

#include "simulator.h"
#include "dijkstra.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

simResult* simulatePacket(graph* g, int src, int dest) {
    if (!g) {
        fprintf(stderr, "error: graph is NULL\n");
        return NULL;
    }

    if (src < 0 || src >= g->num_nodes || dest < 0 || dest >= g->num_nodes) {
        fprintf(stderr, "error: invalid source or destination router\n");
        return NULL;
    }

    //allocate the result structure
    simResult* result = (simResult*)malloc(sizeof(simResult));
    if (!result) {
        fprintf(stderr, "error: failed to allocate memory for simResult\n");
        exit(1);
    }

    //call Dijkstra's algorithm to find the shortest path
    pathResult* pathRes = dijkstraShortestPath(g, src, dest);
    if (!pathRes) {
        fprintf(stderr, "error: dijkstraShortestPath returned NULL\n");
        free(result);
        return NULL;
    }

    //check if destination is reachable
    if (!pathRes->reachable) {
        result->reachable = 0;
        result->pathLength = 0;
        result->path = NULL;
        result->totalCost = -1;
        pathResultFree(pathRes);
        return result;
    }

    //copy the path from dijkstra result
    result->pathLength = pathRes->pathLength;
    result->totalCost = pathRes->totalCost;
    result->reachable = 1;

    //allocate memory for path array
    result->path = (int*)malloc(pathRes->pathLength * sizeof(int));
    if (!result->path) {
        fprintf(stderr, "error: failed to allocate memory for path array\n");
        exit(1);
    }

    //copy the path data
    memcpy(result->path, pathRes->path, pathRes->pathLength * sizeof(int));

    //log each hop with formatting
    printf("\npacket simulation from router %d to router %d\n", src, dest);
    printf("path: ");
    for (int i = 0; i < result->pathLength; i++) {
        printf("%d", result->path[i]);
        if (i < result->pathLength - 1) {
            printf(" -> ");
        }
    }
    printf("\n\n");

    //log hop-by-hop details with link costs
    for (int i = 0; i < result->pathLength - 1; i++) {
        int currentRouter = result->path[i];
        int nextRouter = result->path[i + 1];

        //find the link cost between current and next router
        int linkCost = 0;
        adjListHead* list = &g->adjList[currentRouter];
        adjNode* node = list->head;
        while (node) {
            if (node->dest == nextRouter) {
                linkCost = node->weight;
                break;
            }
            node = node->next;
        }

        printf("hop %d: packet at router %d -> forwarding to router %d (link cost: %d)\n",
               i + 1, currentRouter, nextRouter, linkCost);
    }

    printf("\ntotal path cost: %d\n", result->totalCost);
    printf("total hops: %d\n\n", result->pathLength);

    //free the Dijkstra result
    pathResultFree(pathRes);

    return result;
}

void simResultPrint(simResult* r) {
    if (!r) {
        fprintf(stderr, "error: simResult is NULL\n");
        return;
    }

    if (!r->reachable) {
        printf("\nsimulation result\n");
        printf("status: unreachable\n");
        printf("destination is not reachable from the source.\n\n");
        return;
    }

    printf("\npacket simulation result\n\n");

    printf("route taken:\n");
    for (int i = 0; i < r->pathLength; i++) {
        if (i > 0) {
            printf("  |\n");
        }
        printf("  [router %d]", r->path[i]);
        if (i < r->pathLength - 1) {
            printf("\n");
        }
    }
    printf("\n\n");

    printf("statistics:\n");
    printf("  total hops: %d\n", r->pathLength);
    printf("  total path cost: %d\n", r->totalCost);
    printf("  status: success\n\n");
}

void simResultFree(simResult* r) {
    if (!r) {
        return;
    }

    if (r->path) {
        free(r->path);
        r->path = NULL;
    }

    free(r);
}
