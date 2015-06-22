//Smith-Waterman algorithm with run time O(n*m) and memory O(3*n*m) 
#include "SW.h"
#include <stdio.h>
#include <string.h>

#define PRINT

int Max(int a,int b){
	return a>b ? a:b;
}

int Max3(int a,int b,int c){
	if(b>c)
		return a>b ? a:b;
	return a>c ? a:c;
}

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m;

	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[n][m];
	int Ix[n][m];
	int Iy[n][m];
		
	for(i=0;i<n;i++){	//O(n)
		H[i][0]=0;
		Ix[i][0]=0;
		Iy[i][0]=0;
	}
	for(j=0;j<m;j++){	//O(m)
		H[0][j]=0;
		Ix[0][j]=0;
		Iy[0][j]=0;
	}
		
	for(i=1;i<n;i++)	//O(n*m)
		for(j=1;j<m;j++){	//O(m)
			Ix[i][j]=Max(H[i-1][j]+W(1), Ix[i-1][j]+W(1));
			Iy[i][j]=Max(H[i][j-1]+W(1), Iy[i][j-1]+W(1));
		
			H[i][j]=Max3(H[i-1][j-1]+s(a[i-1],b[j-1]), Ix[i][j], Iy[i][j]);
			H[i][j]=Max(H[i][j],0);
		}
	
	#if defined(PRINT) 
	printf("H  -\t");
	for(j=0;j<m;j++)
		printf("%c\t",b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("-  ");
		else
			printf("%c: ",a[i-1]);
		for(j=0;j<m;j++)
			printf("%d\t",H[i][j]);
		printf("\n");
	}
	#endif
	/******** the code here check the Ix and Iy arrays *********
	printf("\nIx -\t");
	for(j=0;j<m;j++)
		printf("%c\t",b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("-  ");
		else
			printf("%c: ",a[i-1]);
		for(j=0;j<m;j++)
			printf("%d\t",Ix[i][j]);
		printf("\n");
	}
	printf("\nIy -\t");
	for(j=0;j<m;j++)
		printf("%c\t",b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("-  ");
		else
			printf("%c: ",a[i-1]);
		for(j=0;j<m;j++)
			printf("%d\t",Iy[i][j]);
		printf("\n");
	}*/
	
	return H[n-1][m-1];
}
