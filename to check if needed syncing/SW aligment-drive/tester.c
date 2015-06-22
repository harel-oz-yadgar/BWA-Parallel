#include <stdio.h>
#include<SW.h>

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
	int score;
	//this example is taken from http://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
	printf("Example1:\n%s\n%s\n\n","AGCACACA","ACACACTA");
	score=SW("AGCACACA","ACACACTA",s1,W1);
	printf("The score is: %d\n",score);
	
	//this example is taken from http://amrita.vlab.co.in/?sub=3&brch=274&sim=1433&cnt=1
	printf("\n\nExample2:\n%s\n%s\n\n","GACTTAC","CGTGAATTCAT");
	score=SW("GACTTAC","CGTGAATTCAT",s2,W2);
	printf("The score is: %d\n",score);
	
	//this example is taken from http://www.slideshare.net/avrilcoghlan/the-smith-waterman-algorithm
	printf("\n\nExample2:\n%s\n%s\n\n","ACCTAAGG","GGCTCAATCA");
	score=SW("ACCTAAGG","GGCTCAATCA",s3,W3);
	printf("The score is: %d\n",score);
	return 0;
}
