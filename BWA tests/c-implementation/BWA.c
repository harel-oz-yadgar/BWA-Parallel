#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Group.h"

#define TRUE 1
#define FALSE 0

void** BWT(char* X,int length);
void circulateString(char* str,int circulations,int length);
void sortStrings(char** SA,int* S,int length);
int C(char a,char* X,int length);
int O(char a,int i,char* B);
int* calculateD(char* W,char* X,int length,char* B);
Group* InexRecur(char* W,int i,int z,int k,int l,int* D,char* X,int length,char* B);
Group* InexactSearch(char* W,int z,char* X,int length,char* B,char* Btag);

/*
int lowerR(char* W);
int upperR(char* W);
*/
void printData(char* X,char* Xtag,char* B,char* Btag,int* S,int length);

int main(){
	char X[100]="googol";
	int length;
	int i;
	void** bwt;
	char *B,*Xtag,*Btag;
	int *S,*D;
	
	//strcpy(X,"googol");	//get string from user

	strcat(X,"$");
	length=strlen(X);
	
	bwt=BWT(X,length);
	B=bwt[0];
	S=bwt[1];
	free(bwt);
	
	Xtag=(char*)malloc((length+1)*sizeof(char));
	for(i=0;i<length;i++)
		Xtag[i]=X[length-i-1];
	Xtag[i]='\0';
	
	bwt=BWT(Xtag,length);
	Btag=bwt[0];
	free(bwt[1]);
	free(bwt);
	
	
	printData(X,Xtag,B,Btag,S,length);	
	
	char* W="lol";
	/*D=calculateD(W,X,length,Btag);
	
	for(i=0;i<strlen(W);i++)
		printf("D(%d)=%d\n",i,D[i]);
	*/	/*
	Group* I=InexactSearch(W,2,X,length,B,Btag);	
	printGroup(I);	*/
		
		
	free(B);
	free(S);
	
	free(Xtag);
	free(Btag);
	//if(I!=NULL)
	//	freeMemory(I);
	
	return 0;
}

Group* InexactSearch(char* W,int z,char* X,int length,char* B,char* Btag){
	int* D=calculateD(W,X,length,Btag);
	Group* I=InexRecur(W,strlen(W)-1,z,1,length-1,D,X,length,B);
	free(D);
	return I;
}

Group* InexRecur(char* W,int i,int z,int k,int l,int* D,char* X,int length,char* B){
	char letter[]={'g','l','o'};
	int numOfLetters=3;
	int j;
	Group* I=NULL;
	
	if(z<D[i])	//mismatch passed
		return NULL;
	if(i<0){
		I=newGroup();
		addElements(I,k,l);
		return I;
	}
	I=unionGroups(I,InexRecur(W,i-1,z-1,k,l,D,X,length,B));
	for(j=0;j<numOfLetters;j++){
		k=C(letter[j],X,length)+O(letter[j],k-1,B)+1;
		l=C(letter[j],X,length)+O(letter[j],l,B);
		if(k<=l){
			I=unionGroups(I,InexRecur(W,i,z-1,k,l,D,X,length,B));
			if(letter[j]==W[i])
				I=unionGroups(I,InexRecur(W,i-1,z,k,l,D,X,length,B));
			else
				I=unionGroups(I,InexRecur(W,i-1,z-1,k,l,D,X,length,B));
		}
	}
	return I;
}

void** BWT(char* X,int length){	//return the BWT transform of string X, B in index 0 and S in index 1
	int i;
	char* B=(char*)malloc((length+1)*sizeof(char));
	int* S=(int*)malloc(length*sizeof(int));
	char** SA;
	void** data=(void*)malloc(2*sizeof(void*));
	//initialize the S array; 
	for(i=0;i<length;i++)
		S[i]=i;
		
	//initialize the Suffix Array 	
	SA=(char**)malloc(length*sizeof(char*));
	for(i=0;i<length;i++){
		SA[i]=(char*)malloc((length+1)*sizeof(char));
		SA[i][0]='\0';
		strcpy(SA[i],X);
		circulateString(SA[i],i,length);//circulate the string X by i 
	}
	sortStrings(SA,S,length);//sort the strings alphabetic, changing SA and S
	
	/*printf to test the code
	for(i=0;i<length;i++)
		printf("%s\n",SA[i]);*/
	
	//copy the last latter of each row in the suffix array into B
	for(i=0;i<length;i++)
		B[i]=SA[i][length-1];
	B[i]='\0';
	
	//free allocations
	for(i=0;i<length;i++)
		free(SA[i]);
	free(SA);
	
	data[0]=B;
	data[1]=S;
	return data;
}

void circulateString(char* str,int circulations,int length){	//circulate string <str> with length <length> by <circulations>  
	int i,j;
	char c;
	for(i=0;i<circulations;i++){
		c=str[0];
		for(j=0;j<length-1;j++)
			str[j]=str[j+1];
		str[length-1]=c;
	}
}

void sortStrings(char** SA,int* S,int length){	//sort strings 
	int i,change,tmp;
	char* tmpStr;
	do{
		change=FALSE;
		for(i=0;i<length-1;i++)
			if(strcmp(SA[i],SA[i+1])>0){
				change=TRUE;
				tmpStr=SA[i+1];
				SA[i+1]=SA[i];
				SA[i]=tmpStr;
				tmp=S[i+1];
				S[i+1]=S[i];
				S[i]=tmp;
			}
	}while(change==TRUE);
}

void swap(void** a,void** b){
	void* tmp;
	tmp=*a;
	*a=*b;
	*b=tmp;
}

int C(char a,char* X,int length){
	int i,counter=0;
	for(i=0;i<length-1;i++)
		if(X[i]<a)
			counter++;
	return counter;
}

int O(char a,int i,char* B){
	int j,counter=0;
	for(j=0;j<=i;j++)
		if(B[j]==a)
			counter++;
	return counter;
}
/*
int lowerR(char* W){
	if(strcmp(W,"")==0)
		return 1;	//the code say it should be 1 but it doesnt look right
	return C(W[0])+O(W[0],lowerR(W+1)-1)+1;
}

int upperR(char* W){
	if(strcmp(W,"")==0)
		return length-1;
	return C(W[0])+O(W[0],upperR(W+1));
}
*/
int* calculateD(char* W,char* X,int length,char* B){
	int k,l,z,i,len;
	int* D;
	k=1;
	l=length-1;
	z=0;
	len=strlen(W);
	D=(int*)malloc(len*sizeof(int));
	for(i=0;i<len;i++){
		k=C(W[i],X,length)+O(W[i],k-1,B)+1;
		l=C(W[i],X,length)+O(W[i],l,B);
		if(k>l){
			k=1;
			l=length-1;
			z=z+1;
		}
		printf("k-%d,l-%d,z-%d\n",k,l,z);
		D[i]=z;
	}
	return D;
}
/*	char w[]={'l','o','l','\0'};
	int z,i,j,len;
	int* D;
	char tmp;
	j=0;
	z=0;
	len=strlen(W);
	D=(int*)malloc(len*sizeof(int));
	for(i=0;i<len;i++){
		tmp=W[i+1];
		W[i+1]='\0';
		/*if(strstr(X,&(W[j]))==NULL){
			z++;
			j=i+1;
		}
		D[i]=z;
		W[i+1]=tmp;*/
/*	}
	return D;
}
*/
void printData(char* X,char* Xtag,char* B,char* Btag,int* S,int length){
	int i;
	printf("X  - %s\nX' - %s\n\n",X,Xtag);
	printf("B  - %s\nB' - %s\n\n",B,Btag);
	
	printf("S = (");
	for(i=0;i<length;i++){
		if(i<length-1)
			printf("%d,",S[i]);
		else
			printf("%d",S[i]);
	}
	printf(")\n\n");
	
	printf("C(g)=%d\nC(l)=%d\nC(o)=%d\n\n",C('g',X,length),C('l',X,length),C('o',X,length));
	
	
	printf(" O | 0 | 1 | 2 | 3 | 4 | 5 | 6\n l ");
	for(i=0;i<length;i++)
		printf("| %d ",O('l',i,B));
	printf("\n o ");
	for(i=0;i<length;i++)
		printf("| %d ",O('o',i,B));
	printf("\n g ");
	for(i=0;i<length;i++)
		printf("| %d ",O('g',i,B));
	printf("\n\n");
	
	printf(" O'| 0 | 1 | 2 | 3 | 4 | 5 | 6\n l ");
	for(i=0;i<length;i++)
		printf("| %d ",O('l',i,Btag));
	printf("\n o ");
	for(i=0;i<length;i++)
		printf("| %d ",O('o',i,Btag));
	printf("\n g ");
	for(i=0;i<length;i++)
		printf("| %d ",O('g',i,Btag));
	printf("\n\n");	
}
