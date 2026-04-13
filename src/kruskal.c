#include<stdlib.h>
#include"unionFind.h"
#include"kruskal.h"
#include<assert.h>
#include<stdio.h>

static int compareEdges(const void* a, const void* b){
    edge* edgeA = (edge*)a;
    edge* edgeB = (edge*)b;
    return edgeA->weight - edgeB->weight; // Sort in ascending order of weight
}

mstResult* kruskal(graph* g){
    int count = 0;
    edge* edges = getAllEdges(g,&count);
    assert(edges); //getAllEdges should never return NULL, it should return an empty array if there are no edges.
    unionFind* uf = ufCreate(g->num_nodes);
    mstResult* result = (mstResult*)malloc(sizeof(mstResult));
    if(!result) return NULL;

    result->edges = (edge*)malloc(sizeof(edge)*(g->num_nodes-1));
    result->totalCost=0;
    result->edgeCount=0;
    int index = 0;
    qsort(edges, count, sizeof(edge), compareEdges);
    for(int i=0;i<count;i++){
        if(ufUnion(uf,edges[i].src,edges[i].dest)==1){
            result->edges[index]=edges[i];
            result->edgeCount++;
            result->totalCost+=edges[i].weight;
            index++;
        }
        if(result->edgeCount==g->num_nodes-1) break;
    }
    ufFree(uf);
    free(edges);
    return result;
}

void mstPrint(mstResult* result){
    assert(result);
    printf("Edges in the MST:\n");
    for(int i=0;i<result->edgeCount;i++){
        edge e = result->edges[i];
        printf("%d->%d, weight: %d\n",e.src,e.dest,e.weight);
    }
    printf("Total cost: %d\n",result->totalCost);
}

void mstFree(mstResult* result){
    if(!result) return;
    free(result->edges);
    free(result);
}