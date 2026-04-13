#include<stdlib.h>
#include<stdio.h>
#include "prim.h"
#include "graph.h"
#include "kruskal.h"
#include "minHeap.h"
#include <stdbool.h>
#include<limits.h>

#define INF INT_MAX

mstResult* primMST(graph* g){
    //we take 0 as the starting point for Prim's algorithm
    if(!g) return NULL;
    int n = g->num_nodes;
    int* key = (int*)malloc(sizeof(int)*n);
    int* parent = (int*)malloc(sizeof(int)*n);
    bool* inMST = (bool*)malloc(sizeof(bool)*n);
    if(!key||!parent||!inMST) return NULL;
    mstResult* result = (mstResult*)malloc(sizeof(mstResult));
    if(!result) {
        free(key);
        free(parent);
        free(inMST);
        return NULL;
    }
    result->edges=(edge*)malloc(sizeof(edge)*(g->num_nodes-1));
    if(!result->edges){
        free(result);
        free(key);
        free(parent);
        free(inMST);
        return NULL;
    }
    result->edgeCount=0;
    result->totalCost=0;
    //initializing all.
    for(int i=0;i<n;i++)
    {
        key[i]=INF;
        parent[i]=-1;
        inMST[i]=false;
    }
    //startIndex is 0
    key[0]=0;

    minHeap* heap = minHeapCreate(n);
    for(int i=0;i<n;i++){
        minHeapInsert(heap,i,key[i]);
    }
    while(!minHeapIsEmpty(heap)){
        heapNode minNode = minHeapExtractMin(heap);
        int u = minNode.node;
        inMST[u]=true;

        if(parent[u]!=-1){
            result->edges[result->edgeCount].src=parent[u];
            result->edges[result->edgeCount].dest=u;
            result->edges[result->edgeCount].weight=key[u];
            result->edgeCount++;
            result->totalCost+=key[u];
        }

        adjNode* curr = g->adjList[u].head;
        while(curr){
            int v = curr->dest;
            int weight = curr->weight;
            if(!inMST[v] && weight < key[v]){
                key[v] = weight;
                parent[v] = u;
                minHeapDecreaseKey(heap,v,weight);
            }
            curr=curr->next;
        }
    }
    
    //free the allocated memory
    free(key);
    free(parent);
    free(inMST);
    minHeapFree(heap);
    return result;
}