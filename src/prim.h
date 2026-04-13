/*
    prim.h
    netroute-sim - Prim's algorithm for finding the Minimum Spanning Tree (MST) of a graph
*/

#ifndef PRIM_H
#define PRIM_H

#include "kruskal.h"
#include "graph.h"

//finds the Minimum Spanning Tree (MST) of a graph using Prim's algorithm
mstResult* primMST(graph* g); 


#endif