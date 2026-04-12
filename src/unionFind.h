/*
    unionFind.h
    netroute-sim - Union-Find (Disjoint Set Union) data structure for network route simulation
*/

#ifndef UNION_FIND_H //header guard

#define UNION_FIND_H

typedef struct unionFind{
    int* parent; //stores the parent of each node, used for path compression, -1 denotes that the node is a parent of itself and the absolute value of the value at the root denotes the size of the set.
} unionFind;

unionFind* ufCreate(int n); //creates a new union-find structure with n elements

int ufFindParent(unionFind* uf, int x); //finds and returns the parent of the set containing x, with path compression, for RAW value use parent[x]

int ufUnion(unionFind* uf, int x, int y); //unites the sets containing x and y, returns 0 if they were already in the same set, 1 otherwise

int ufConnected(unionFind* uf, int x, int y); //checks if x and y are in the same set, returns 1 if they are, 0 otherwise

void ufFree(unionFind* uf); //frees the memory allocated for the union-find structure. Frees in this order: parent[], then the struct itself

#endif