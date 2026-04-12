#include<stdlib.h>
#include"minHeap.h"

static void swapNodes(minHeap* heap, int i, int j){
    //swap the nodes in the data array
    heapNode temp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = temp;
    //update the positions in the pos array
    heap->pos[heap->data[i].node] = i;
    heap->pos[heap->data[j].node] = j;
}

static void heapifyUp(minHeap* heap, int i){
    int parent = (i-1)/2;
    if(i>0 && heap->data[i].weight < heap->data[parent].weight){
        swapNodes(heap, i, parent);
        heapifyUp(heap, parent);
    }
}

static void heapifyDown(minHeap* heap, int i){
    int left = 2*i + 1;
    int right = 2*i + 2;
    int smallest = i;
    if(left < heap->size && heap->data[left].weight < heap->data[smallest].weight){
        smallest = left;
    }
    if(right < heap->size && heap->data[right].weight < heap->data[smallest].weight){
        smallest = right;
    }
    if(smallest != i){
        swapNodes(heap, i, smallest);
        heapifyDown(heap, smallest);
    }
}

minHeap* minHeapCreate(int capacity){
    if(capacity<=0) return NULL;
    minHeap* heap = (minHeap*)malloc(sizeof(minHeap));
    if(!heap) return NULL;
    heap->capacity=capacity;
    heap->data = (heapNode*)malloc(capacity*sizeof(heapNode));
    if(!heap->data){
        free(heap);
        return NULL;
    }
    heap->pos = (int *)malloc(sizeof(int)*capacity);
    if(!heap->pos)
    {
        free(heap->data);
        free(heap);
        return NULL;
    }
    for(int i=0;i<capacity;i++){
        heap->pos[i]=-1;//position of all is initially -1;
    }
    heap->size=0;
    return heap;
}

void minHeapInsert(minHeap* heap, int node, int weight){
    if(heap->size >= heap->capacity) return;
    if(minHeapContains(heap, node)) return;  //prevent duplicates
    int i = heap->size;
    heap->data[i].node=node;
    heap->data[i].weight=weight;
    heap->pos[node]=i;
    heap->size++;
    heapifyUp(heap, i);
}

heapNode minHeapExtractMin(minHeap* heap){
    if(heap->size <= 0) return (heapNode){-1, -1};
    heapNode minNode = heap->data[0];
    heap->pos[minNode.node]=-1;//mark as removed
    heap->size--;
    if(heap->size > 0){
        heap->data[0] = heap->data[heap->size];
        heap->pos[heap->data[0].node]=0;
        heapifyDown(heap, 0);
    }
    return minNode;
}

void minHeapDecreaseKey(minHeap* heap, int node, int newWeight){
    int i = heap->pos[node];
    if(i==-1 || heap->data[i].weight <= newWeight) return;
    heap->data[i].weight = newWeight;
    heapifyUp(heap, i);
}

int minHeapIsEmpty(minHeap* heap){
    return heap->size == 0;
}

int minHeapContains(minHeap* heap, int node){
    return heap->pos[node] != -1;
}

void minHeapFree(minHeap* heap){
    if(!heap) return;
    free(heap->data);
    free(heap->pos);
    free(heap);
}