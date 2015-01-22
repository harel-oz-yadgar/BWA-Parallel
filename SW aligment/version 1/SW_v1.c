//Smith-Waterman algorithm with run time O(n^2*m+n*m^2) and memory O(n*m) 
#include "SW.h"
#include <stdio.h>
#include <string.h>

int Max(int a,int b){
	return a>b ? a:b;
}

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,k,l,n,m;
	
	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[n][m];
		
	for(i=0;i<n;i++)	//O(n)
		H[i][0]=0;
	for(j=0;j<m;j++)	//O(m)
		H[0][j]=0;
		
	for(i=1;i<n;i++)	//O(n*m*(n+m))=O(n^2*m+n*m^2)
		for(j=1;j<m;j++){	//O(m*(n+m))
			H[i][j]=0;
			H[i][j]=Max(H[i][j],H[i-1][j-1]+s(a[i-1],b[j-1]));
			for(k=1;k<=i;k++)	//O(n)
				H[i][j]=Max(H[i][j],H[i-k][j]+W(k));
			for(l=1;l<=j;l++)	//O(m)
				H[i][j]=Max(H[i][j],H[i][j-l]+W(l));
		}
	return H[n-1][m-1];
}
