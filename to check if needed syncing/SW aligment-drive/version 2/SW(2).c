//this is the basic Smith-Waterman algorithm with run time of O(n*m) and memory of O(n*m) 
#include <stdio.h>

#define INSERTION 1
#define DELETION 1

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m;
	int **H,**Ix,**Iy;
	
	n=strlen(a);
	m=strlen(b);
	H=(int**)malloc(n*sizeof(int*));
	Ix=(int**)malloc(n*sizeof(int*));
	Iy=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		H[i]=(int*)malloc(m*sizeof(int));
		Ix[i]=(int*)malloc(m*sizeof(int));
		Iy[i]=(int*)malloc(m*sizeof(int));
	}
		
	for(i=0;i<n;i++){
		H[i][0]=0;
		Ix[i][0]=0;
		Iy[i][0]=0;
	}
	for(j=0;j<m;j++){
		H[0][j]=0;
		Ix[0][j]=0;
		Iy[0][j]=0;
	}
		
	for(i=1;i<n;i++)
		for(j=1;j<m;j++){
			H[i][j]=0;
			H[i][j]=Max(H[i][j],H[i-1][j-1]+s(a[i],b[j]));
			H[i][j]=Max(H[i][j],Ix[i-1][j-1]+s(a[i],b[j]));
			H[i][j]=Max(H[i][j],Iy[i-1][j-1]+s(a[i],b[j]));
			
			Ix[i][j]=Max(H[i−1,j]-DELETION,Ix[i−1][j]−INSERTION);
			Iy[i][j]=Max(H[i][j−1]−DELETION,Iy[i][j−1]−INSERTION);
		}
		
	for(i=0;i<n;i++){
		for(j=0;j<m;j++)
			printf("%d ",H[i][j]);
		printf("\n");
	}
	
	return H[n-1][m-1];
}

int s1(char a,char b){
	if(a==b)
		return 2;
	return -1;
}

int W1(int i){
	return -i;
}

int main(){
	int score=SW("ACACACTA","AGCACACA",s1,W1);
}