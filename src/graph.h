/*
    graph.h
    netroute-sim - Adjacency list graph implementation for network route simulation
*/

#ifndef GRAPH_H //header guard



#define GRAPH_H

typedef struct adjnode {
    int dest;          //index of the neighbouring router in the graph's adjacency list
    int weight;          //cost of this connection 
    struct adjnode* next;//pointer to the next neighbour (linked list)
} adjNode;

typedef struct adjListHead {
    adjNode* head; //pointer to the head of the linked list of neighbours for a given router
    adjNode* tail; //pointer to the tail of the linked list of neighbours for a given router (for efficient appending)
} adjListHead;

typedef struct graph {
    int num_nodes; //total number of routers in the network
    int num_edges; //total number of connections between routers in the network
    adjListHead* adjList; //array of adjacency lists
} graph;

typedef struct edge {
    int src;    //router on one end
    int dest;   //router on the other end
    int weight; //cost of the connection between src and dest
} edge;

//allocates a new empty graph with a specified number of nodes (routers)
graph* createGraph(int num_nodes);

//adds a connection between two routers
void addEdge(graph* g, int src, int dest, int weight);

//retrieves all edges in the graph
edge* getAllEdges(graph* g, int* endCount);

//frees the memory allocated for the graph
void graphFree(graph* g);

//prints the graph
void graphPrint(graph* g);



#endif 