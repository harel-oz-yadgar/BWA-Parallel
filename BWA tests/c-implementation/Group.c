#include"Group.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

void swapElements(int* a,int* b){	//swap between elements in the group, usefull for sorting
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

Group* newGroup(){	//create new group
	Group* g=(Group*)malloc(sizeof(Group));
	g->elements=(int*)malloc(INITIAL_SIZE*sizeof(int));
	g->numOfElements=0;
	g->maxElements=INITIAL_SIZE;
	return g;
}

void addElement(Group* g,int element){	//adding one element to the group
	int i;
	int* tmp;
	if(g->numOfElements==0){
		g->numOfElements++;
		g->elements[0]=element;
		return;
	}
	//check if the element is already part of the group
	for(i=0;i<g->numOfElements;i++)
		if(g->elements[i]==element)
			return;
	//add more size to the group if needed		
	if(g->numOfElements==g->maxElements){	
		g->maxElements+=GROWTH_FACTOR;
		tmp=(int*)malloc(g->maxElements*sizeof(int));
		memcpy(tmp,g->elements,g->numOfElements*sizeof(int));
		free(g->elements);
		g->elements=tmp;
	}
	//add the new element and bubble sort 
	g->elements[g->numOfElements]=element;
	for(i=g->numOfElements;i>0;i--)	
		if(g->elements[i]<g->elements[i-1])
			swapElements(&(g->elements[i]),&(g->elements[i-1]));	
	g->numOfElements++;
}

void addElements(Group* g,int from,int to){
	int element,i,j;
	int* tmp;	
	//check if needed to add size to the group
	if( (g->numOfElements+to-from+1)>=g->maxElements ){
		g->maxElements+=GROWTH_FACTOR+(to-from+1);
		tmp=(int*)malloc(g->maxElements*sizeof(int));
		memcpy(tmp,g->elements,g->numOfElements*sizeof(int));
		free(g->elements);
		g->elements=tmp;
	}
	//add the elements and bubble sort
	for(element=from;element<=to;element++){
		for(i=0;i<g->numOfElements;i++)
			if(g->elements[i]==element)
				break;
		if(i==g->numOfElements){	//the element is not in the group
			g->elements[g->numOfElements]=element;
			for(j=g->numOfElements;j>0;j--)	
				if(g->elements[j]<g->elements[j-1])
					swapElements(&(g->elements[j]),&(g->elements[j-1]));	
			g->numOfElements++;
		}
	}
}

Group* unionGroups(Group* A,Group* B){
	int eleA,eleB,elements;
	int* tmp;
	if(A==NULL && B==NULL)
		return NULL;
	if(A==NULL)
		return B;
	if(B==NULL)
		return A;
	
	if(A->numOfElements==0){
		freeMemory(A);
		return B;
	}
	if(B->numOfElements==0){
		freeMemory(B);
		return A;
	}
	
	for(eleA=0,eleB=0,elements=0; eleA < A->numOfElements && eleB < B->numOfElements ;elements++){
		if( A->elements[eleA] == B->elements[eleB] ){	//if the two groups have the same element
			eleA++;
			eleB++;
		}
		else if( A->elements[eleA] < B->elements[eleB] )	//if the element in group A is smaller
			eleA++;
		else if( A->elements[eleA] > B->elements[eleB] )	//if the element in group B is smaller
			eleB++;
	}
	if( eleA!=A->numOfElements )	//add the elements left in A
		elements+= A->numOfElements-eleA;
	else if( eleB!=B->numOfElements )	//add the elements left in B
		elements+= B->numOfElements-eleB;
	
	//create the new union group	
	Group* u=(Group*)malloc(sizeof(Group));
	u->maxElements=elements+GROWTH_FACTOR;
	u->elements=(int*)malloc(u->maxElements*sizeof(int));
	u->numOfElements=0;
	
	for(eleA=0,eleB=0; eleA < A->numOfElements && eleB < B->numOfElements ;u->numOfElements++){
		if( A->elements[eleA] == B->elements[eleB] ){	//if the two groups have the same element
			u->elements[u->numOfElements]=A->elements[eleA];
			eleA++;
			eleB++;
		}
		else if( A->elements[eleA] < B->elements[eleB] ){	//if the element in group A is smaller
			u->elements[u->numOfElements]=A->elements[eleA];
			eleA++;
		}
		else if( A->elements[eleA] > B->elements[eleB] ){	//if the element in group B is smaller
			u->elements[u->numOfElements]=B->elements[eleB];
			eleB++;
		}
	} 
	for(;eleA<A->numOfElements; eleA++,u->numOfElements++)	//add the elements left in A
		u->elements[u->numOfElements]=A->elements[eleA];
	for(;eleB<B->numOfElements; eleB++,u->numOfElements++)	//add the elements left in B
		u->elements[u->numOfElements]=B->elements[eleB];
	
	freeMemory(A);
	freeMemory(B);
	return u;
}

void freeMemory(Group* g){
	free(g->elements);
	free(g);
}

void printGroup(Group* g){	//print the group
	int i;
	if(g==NULL){
		printf("NULL\n");
		return;
	}
	printf("{");
	for(i=0;i<g->numOfElements;i++){
		printf("%d",g->elements[i]);
		if(i!=g->numOfElements-1)
			printf(", ");
	}
	printf("} max=%d\n",g->maxElements);
}
