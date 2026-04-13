/*
    loader.h
    netroute-sim - Topology file loader for reading network graphs from disk
*/

#ifndef LOADER_H
#define LOADER_H

#include "graph.h"

//loads a graph from a topology file
//returns a pointer to the created graph, or NULL if file cannot be opened
graph* loadGraphFromFile(const char* filepath);

#endif
