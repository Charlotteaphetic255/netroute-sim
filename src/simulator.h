/*
    simulator.h
    netroute-sim - packet transmission simulation using Dijkstra's shortest path
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "graph.h"

typedef struct {
    int* path;       //array of router IDs representing the packet's path
    int pathLength;  //number of hops in the path (number of routers visited)
    int totalCost;   //total cost of the path
    int reachable;   //1 if destination is reachable, 0 otherwise
} simResult;

//simulates a packet transmission from src to dest using Dijkstra's shortest path
//logs each hop to stdout showing the router transitions and link costs
//returns a simResult struct containing the path, cost, and reachability flag
//caller must free result with simResultFree()
simResult* simulatePacket(graph* g, int src, int dest);

//prints the simulation result in a nicely formatted hop-by-hop output
//shows the complete path taken by the packet and total transmission cost
void simResultPrint(simResult* r);

//frees all memory allocated for a simResult, including the path array
void simResultFree(simResult* r);

#endif
