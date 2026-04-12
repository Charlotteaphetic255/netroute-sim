#include<stdlib.h>
#include"graph.h"

graph* createGraph(int num_nodes) {
    graph* g = (graph*)malloc(sizeof(graph));
    g->num_nodes = num_nodes;
    g->num_edges = 0;
    g->adjList = (adjListHead*)malloc(num_nodes * sizeof(adjListHead));
    for (int i = 0; i < num_nodes; i++) {
        g->adjList[i].head = NULL;
        g->adjList[i].tail = NULL;
    }
    return g;
}

void addEdge(graph *g, int src, int dest, int weight){
    // Check if edge already exists in forward direction
    adjNode* existing = g->adjList[src].head;
    while(existing){
        if(existing->dest == dest){
            // Edge exists, update weight instead of creating duplicate
            existing->weight = weight;
            
            // Also update reverse edge
            adjNode* reverseExisting = g->adjList[dest].head;
            while(reverseExisting){
                if(reverseExisting->dest == src){
                    reverseExisting->weight = weight;
                    return;
                }
                reverseExisting = reverseExisting->next;
            }
            return;
        }
        existing = existing->next;
    }
    
    // Edge doesn't exist, create new nodes
    adjNode* newNode = (adjNode*)malloc(sizeof(adjNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    if(!g->adjList[src].head){
        g->adjList[src].head=g->adjList[src].tail=newNode;
    }else{
        g->adjList[src].tail->next=newNode;
        g->adjList[src].tail=newNode;
    }
    
    //for undirected graph, add the reverse edge as well
    adjNode* reverseNode = (adjNode*)malloc(sizeof(adjNode));
    reverseNode->dest = src;
    reverseNode->weight = weight;
    reverseNode->next = NULL;
    if(!g->adjList[dest].head){
        g->adjList[dest].head=g->adjList[dest].tail=reverseNode;
    }else{
        g->adjList[dest].tail->next=reverseNode;
        g->adjList[dest].tail=reverseNode;
    }

    g->num_edges++;
}

edge* getAllEdges(graph* g, int* endCount){
    edge* edges = (edge*)malloc(g->num_edges * sizeof(edge));
    int index = 0;
    for(int i=0; i<g->num_nodes; i++){
        adjNode* current = g->adjList[i].head;
        while(current){
            if(i < current->dest){  //to avoid duplicates in undirected graph
                edges[index].src = i;
                edges[index].dest = current->dest;
                edges[index].weight = current->weight;
                index++;
            }
            current = current->next;
        }
    }
    *endCount = index;  // Return actual count of unique edges
    return edges;
}

void graphFree(graph* g){
    for(int i=0; i<g->num_nodes; i++){
        adjNode* current = g->adjList[i].head;
        while(current){
            adjNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(g->adjList);
    free(g);
}

void graphPrint(graph* g){
    for(int i=0; i<g->num_nodes; i++){
        adjNode* current = g->adjList[i].head;
        printf("Router %d ",i);
        while(current){
            printf(" -> (Dest: %d, Weight: %d)", current->dest, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}