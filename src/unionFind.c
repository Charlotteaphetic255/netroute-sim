#include<stdlib.h>
#include"unionFind.h"

unionFind* ufCreate(int n){
    if(n<=0) return NULL;
    unionFind* uf=(unionFind*)malloc(sizeof(unionFind));
    if(!uf) return NULL; //check for malloc failure
    uf->parent=(int *)malloc(n * sizeof(int));
    if(!uf->parent){ //check for malloc failure
        free(uf);
        return NULL;
    }
    for(int i=0;i<n;i++){
        uf->parent[i]=-1; //-1 denotes the element is a parent of itself.
    }
    return uf;
}

int ufFindParent(unionFind* uf, int x){
    if(uf->parent[x]<0) return x;
    return uf->parent[x]=ufFindParent(uf,uf->parent[x]);
}

int ufUnion(unionFind* uf, int x, int y){
    int rootX = ufFindParent(uf, x);
    int rootY = ufFindParent(uf, y);
    if(rootX == rootY) return 0; //already in the same set
    if(uf->parent[rootX] < uf->parent[rootY]) //size of X > size of Y
    {
        uf->parent[rootX]+= uf->parent[rootY];
        uf->parent[rootY]=rootX;
    }else if(uf->parent[rootY] < uf->parent[rootX]) //size of Y > size of X
    {
        uf->parent[rootY]+=uf->parent[rootX];
        uf->parent[rootX]=rootY;
    }else //size of X == size of Y, tie break by making rootY the parent
    {
        uf->parent[rootY]+=uf->parent[rootX];
        uf->parent[rootX]=rootY;
    }
    return 1;
}

int ufConnected( unionFind* uf, int x, int y){
    int parentX = ufFindParent(uf, x);
    int parentY = ufFindParent(uf, y);
    return parentX == parentY ? 1 : 0;
}

void ufFree(unionFind* uf){
    if(!uf) return;
    free(uf->parent);
    free(uf);
}