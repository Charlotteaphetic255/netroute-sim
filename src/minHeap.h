/*
    minHeap.h
    netroute-sim - Min-Heap data structure for network route simulation
*/

#ifndef MIN_HEAP_H

#define MIN_HEAP_H

typedef struct heapNode {
    int node; //index of the router in the graph
    int weight; //cost of the connection to this router, used for priority in the min-heap
} heapNode;

typedef struct minHeap {
    heapNode* data;//array storing heap nodes
    int* pos; //pos[node] gives the current index of the node in the heap array
    int size; //current number of elements in the heap
    int capacity; //maximum capacity of the heap
} minHeap;

minHeap* minHeapCreate(int capacity); //creates a new min-heap with the given capacity

void minHeapInsert(minHeap* heap, int node, int weight); //inserts a new node with the given weight into the min-heap

heapNode minHeapExtractMin(minHeap* heap); //extracts and returns the node with the minimum weight from the min-heap

void minHeapDecreaseKey(minHeap* heap, int node, int newWeight); //decreases the weight of the given node in the min-heap to newWeight

int minHeapIsEmpty(minHeap* heap); //returns 1 if the min-heap is empty, 0 otherwise. Called at the top of Prim's and Dijikstra's algorithms main loop to check if we have processed all nodes

int minHeapContains(minHeap* heap, int node); //returns 1 if the min-heap contains the given node, 0 otherwise. Used in Prim's and Dijikstra's algorithms to check if a node is still in the min-heap

void minHeapFree(minHeap* heap); //frees the memory allocated for the min-heap, data[], pos[], then the struct itself.

#endif