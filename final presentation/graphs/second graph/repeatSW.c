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
	//srand(time(NULL));
	int score,i,j,random;
	//the max capability of my home computer is 340000
	int numOfExamples=2;
	int sampleLength=5*2*2*2*2;
	char examples[numOfExamples][sampleLength+1];
	int times=100000;
	
	//
	for(i=0;i<numOfExamples;i++){
		for(j=0;j<sampleLength;j++){
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
		examples[i][sampleLength]='\0';
		//printf("%s\n",examples[i]);
	}

	//for each two samples activate the Smith Waterman aligment
	clock_t tic = clock();
	time_t start=time(NULL);
	for(i=0;i<times;i++)
		score=SW(examples[0],examples[1],s1,W1);
	time_t end=time(NULL);
	clock_t toc = clock();
	printf("%lf\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	//printf("%lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
}
