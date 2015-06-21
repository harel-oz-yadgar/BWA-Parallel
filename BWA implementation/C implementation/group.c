#include"group.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>


/*typedef struct{
int** elements;
int numOfElements,maxElements;
}Group;*/


Group* newGroup(){
	Group* g=(Group*)malloc(sizeof(Group));
	if(g==NULL)
		return NULL;
		
	g->elements=(int**)malloc(INITIAL_SIZE*sizeof(int*));
	if(g->elements==NULL){
		free(g);
		return NULL;
	}
	
	g->numOfElements=0;
	g->maxElements=INITIAL_SIZE;
}

void addElements(Group* g,int k,int l){
	int i;
	int **anchor;

	if(g->numOfElements==0){
		g->elements[0]=(int*)malloc(2*sizeof(int));
		if(g->elements[0]==NULL)
			return;
		g->elements[0][0]=k;
		g->elements[0][1]=l;
		g->numOfElements++;
		return;
	}

	if(g->elements[0][0]>k){	//new first k,l
		shiftRight(g,0);
		g->elements[0]=(int*)malloc(2*sizeof(int));
		if(g->elements[0]==NULL)
			return;
		g->elements[0][0]=k;
		g->elements[0][1]=l;
		g->numOfElements++;
		rebalance(g,0);
		return;
	}

	
	//insert in sorted manner using k
	for(i=0;i<g->numOfElements;i++){
		if(g->elements[i][0]<k)	//didnt locate the spot yet
			continue;
			
		if(g->elements[i][0]==k){	//locate the spot
			if(g->elements[i][1]>=l)	//if k,l is already in the spot return
				return;
			//l[i]<l than take over the spot and rebalance from the spot 
			g->elements[i][1]=l;
			rebalance(g,i);
			return;
		}
		
		if(g->elements[i][0]>k){	//we passed the wanted spot
			shiftRight(g,i);
			g->elements[i]=(int*)malloc(2*sizeof(int));
			if(g->elements[i]==NULL)
				return;
			g->elements[i][0]=k;
			g->elements[i][1]=l;
			g->numOfElements++;
			rebalance(g,i-1);
			return;	
		}
	}
		
	//if we got here k>all k[i]
	if(g->numOfElements==g->maxElements){
		anchor=g->elements;
		g->maxElements+=GROWTH_FACTOR;
		g->elements=(int**)malloc(g->maxElements*sizeof(int*));
		for(i=0;i<g->numOfElements;i++)
			g->elements[i]=anchor[i];
		free(anchor);
	}
	i=g->numOfElements;
	g->elements[i]=(int*)malloc(2*sizeof(int));
	if(g->elements[i]==NULL)
		return;
	g->elements[i][0]=k;
	g->elements[i][1]=l;
	g->numOfElements++;
	rebalance(g,i-1);	
}

void shiftRight(Group* g,int index){//push all the elements starting from <index> to the right
	int** anchor;
	int i;
	
	if(g->numOfElements==g->maxElements){
		anchor=g->elements;
		g->maxElements+=GROWTH_FACTOR;
		g->elements=(int**)malloc(g->maxElements*sizeof(int*));
		for(i=0;i<g->numOfElements;i++)
			g->elements[i]=anchor[i];
		free(anchor);
	}
	
	for(i=g->numOfElements;i>index;i--)
		g->elements[i]=g->elements[i-1];
}

void shiftLeft(Group* g,int index){//push all the elements starting from <index>+1 to the left
	free(g->elements[index]);
	for(;index<g->numOfElements-1;index++)
		g->elements[index]=g->elements[index+1];
	g->numOfElements--;
}

void rebalance(Group* g,int index){
	int i;
	for(i=index;i<g->numOfElements-1;i++){
		if(g->elements[i][1]<g->elements[i+1][0])	//k[i],l[i],k[i+1],l[i+1]
			return rebalance(g,i+1);
		
		if(g->elements[i][1]==g->elements[i+1][0]){ //k[i],l[i]=k[i+1],l[i+1] => k[i],l[i+1]
			g->elements[i][1]=g->elements[i+1][1];
			shiftLeft(g,i+1);
			return rebalance(g,i);			
		}

		//its certain that k[i+1]<l[i]
		if(g->elements[i][1]<=g->elements[i+1][1]){	//k[i],k[i+1],l[i],l[i+1] or k[i],k[i+1],l[i]=l[i+1] => k[i],l[i+1]
			g->elements[i][1]=g->elements[i+1][1];
			shiftLeft(g,i+1);
			return rebalance(g,i);
		}
		
		//k[i],k[i+1],l[i+1],l[i]

		shiftLeft(g,i+1);
		i--;	
	}
}

Group* unionGroups(Group* A,Group* B){
	int i;
	if(A==NULL && B==NULL)
		return NULL;
	if(A==NULL)
		return B;
	if(B==NULL)
		return A;
	
	for(i=0;i<B->numOfElements;i++)
		addElements(A,B->elements[i][0],B->elements[i][1]);
	freeMemory(B);
	return A;
}

void freeMemory(Group* g){
	int i;
	for(i=0;i<g->numOfElements;i++)
		free(g->elements[i]);
	free(g->elements);
	free(g);
}

void printGroup(Group* g){
	int i;
	if(g==NULL){
		printf("NULL\n");
		return;
	}
	printf("elements: %d, max size: %d\n",g->numOfElements,g->maxElements);
	if(g->numOfElements==0)
		return;
	
	for(i=0;i<g->numOfElements-1;i++)
		printf("[%d,%d], ",g->elements[i][0],g->elements[i][1]);
	printf("[%d,%d]\n",g->elements[i][0],g->elements[i][1]);
}
