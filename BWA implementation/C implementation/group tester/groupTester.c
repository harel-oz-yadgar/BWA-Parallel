#include"group.h"
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include<stdio.h>

int main(){
	srand(time(NULL));
	int i,k,l;
	Group* g=newGroup();
	/*
	addElements(g,4,5);
	addElements(g,0,3);//new head
	addElements(g,10,12);//new tail
	addElements(g,8,8);//middle
	addElements(g,8,10);//expand exsisting 
	addElements(g,8,9);//expand bigger than himself
	addElements(g,11,11);//already located -- not handled yet
	addElements(g,-20,1);
	
	addElements(g,13,14);
	addElements(g,15,16);
	addElements(g,16,17);
	
	addElements(g,6,19);
	addElements(g,5,6);
	
	addElements(g,-40,-10);
	addElements(g,-10,6);*/
	
	for(i=0;i<20;i++){
		k=rand()%30;
		do{
			l=rand()%30;
		}while(l<k);
		printf("adding %d,%d\n",k,l);
		addElements(g,k,l);
		printGroup(g);
	}
	
	printGroup(g);
	freeMemory(g);
	return 0;
}
