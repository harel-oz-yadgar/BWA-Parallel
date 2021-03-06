//Smith-Waterman algorithm with run time O(n*m) and memory O(2*m) 
#include "SW.h"
#include <stdio.h>
#include <string.h>

int Max(int a,int b){
	return a>b ? a:b;
}

int Max3(int a,int b,int c){
	if(b>c)
		return a>b ? a:b;
	return a>c ? a:c;
}

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m,Hdiagonal,tmp;

	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[m];
	int Ix[m];
	int Iy;
	
	for(j=0;j<m;j++){	//O(m)
		H[j]=0;
		Ix[j]=0;
	}
	
	for(i=1;i<n;i++){	//O(n*m)
		Hdiagonal=H[0];
		Iy=0;
		for(j=1;j<m;j++){	//O(m)
			Ix[j]=Max(H[j]+W(1), Ix[j]+W(1));
			Iy=Max(H[j-1]+W(1), Iy+W(1));
			tmp=H[j];
			H[j]=Max3(Hdiagonal+s(a[i-1],b[j-1]), Ix[j], Iy);
			H[j]=Max(H[j],0);
			Hdiagonal=tmp;
		}
	}
	return H[m-1];
}
