/*
    dijkstra.h
    netroute-sim Dijkstra's algorithm for finding the shortest path in a graph
*/

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"


typedef struct pathResult{
    int* path; //sequence of routers [src,...,dest]
    int pathLength; //number of routers in the path
    int totalCost; //total cost of the path
    int reachable; //1 if the destination is reachable, 0 incase of disconnected graph
} pathResult;

pathResult* dijkstraShortestPath(graph* g, int src, int dest); //finds the shortest path from src to dest using Dijkstra's algorithm

void pathResultPrint(pathResult* result); //prints the path, total cost, and reachability of the destination

void pathResultFree(pathResult* result); //frees the memory allocated for the path result, including the path array and the struct itself.

#endif