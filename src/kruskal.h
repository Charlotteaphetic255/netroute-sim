/*
    kruskal.h
    netroute-sim - Kruskal's algorithm for finding the Minimum Spanning Tree (MST) of a graph
*/

#ifndef KRUSKAL_H

#define KRUSKAL_H

#include "graph.h"

typedef struct mstResult{
    edge* edges; //mst edges that were selected
    int edgeCount; //always numNodes - 1
    int totalCost; //sum of all selected edge weights
} mstResult; 


//Returns the result of the MST algorithm, which includes the selected edges, their count, and the total cost of the MST.
mstResult* kruskal(graph* g);


// Prints the edges included in the MST and the total cost of the MST.
void mstPrint(mstResult* result);

// Frees the memory allocated for the MST result, including the array of edges.
void mstFree(mstResult* result);


#endif