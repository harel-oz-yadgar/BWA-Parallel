#include "group.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	int i;
	/*
	Group* g=newGroup();
	printGroup(g);
	
	for(i=20;i<35;i++){
		addElement(g,i);
		printGroup(g);
	}
	for(i=0;i<=15;i++){
		addElement(g,i);
		printGroup(g);
	}
	addElement(g,-3);
	printGroup(g);
	
	
	addElements(g,-5,10);
	printGroup(g);	
	
	addElements(g,5,16);
	printGroup(g);
	
	freeMemory(g);*/
	
	Group* A=newGroup();
	Group* B=newGroup();
	addElements(A,0,5);
	addElements(A,6,11);
	addElements(A,20,30);
	printGroup(A);
	addElements(B,10,14);
	addElements(B,-8,1);
	addElements(B,3,7);
	printGroup(B);
	
	Group* u=unionGroups(A,B);
	printGroup(u);
	freeMemory(u);
}
