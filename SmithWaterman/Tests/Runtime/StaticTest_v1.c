#include <stdio.h>
#include "SW.h"
#include <time.h>

int s1(char a,char b){
	return a==b ? 2:-1;
}
int W1(int i){
	return -i;
}

int s2(char a,char b){
	return a==b ? 5:-3;
}
int W2(int i){
	return -4*i;
}

int s3(char a,char b){
	return a==b ? 2:-1;
}
int W3(int i){
	return -2*i;
}
int main(){
	srand(time(NULL));
	int score,i,j,random;
	int numOfExamples=340000;//this is the max capability of my home computer
	char examples[numOfExamples][36];
	
	//create <numOfExamples> string using A,C,G and T with length of 35 characters
	for(i=0;i<numOfExamples;i++){
		for(j=0;j<35;j++){
			random=rand()%4;
			if(random==0)
				examples[i][j]='A';
			if(random==1)
				examples[i][j]='C';
			if(random==2)
				examples[i][j]='G';
			if(random==3)
				examples[i][j]='T';
		}
		examples[i][35]='\0';
	}

	//for each following samples activate the Smith Waterman aligment
	clock_t tic = clock();
	for(i=1;i<numOfExamples;i++)
		score=SW(examples[i],examples[i-1],s1,W1);
	
	clock_t toc = clock();
	printf("%lf\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return 0;
}
