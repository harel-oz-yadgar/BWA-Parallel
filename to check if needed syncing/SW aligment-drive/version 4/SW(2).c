//this is the Smith-Waterman algorithm with run time of O(n*m) and memory of O(3*m) 
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
	int i,j,n,m,Hdiagonal,tmp;

	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[m];
	int Ix[m];
	int Iy[m];
	
	for(j=0;j<m;j++){	//O(m)
		H[j]=0;
		Ix[j]=0;
		Iy[j]=0;
	}
	
	#if defined(PRINT)
	printf("H  -\t");
	for(j=0;j<m;j++)
		printf("%c\t",b[j]);
	printf("\n-  ");
	for(j=0;j<m;j++)
			printf("%d\t",H[j]);
	printf("\n");	
	#endif
	for(i=1;i<n;i++){	//O(n*m)
		#if defined(PRINT) 
		printf("%c: 0\t",a[i-1]);
		#endif
		Hdiagonal=H[0];
		for(j=1;j<m;j++){	//O(m)
			Ix[j]=Max(H[j]+W(1), Ix[j]+W(1));
			Iy[j]=Max(H[j-1]+W(1), Iy[j-1]+W(1));
			tmp=H[j];
			H[j]=Max3(Hdiagonal+s(a[i-1],b[j-1]), Ix[j], Iy[j]);
			H[j]=Max(H[j],0);
			Hdiagonal=tmp;
			#if defined(PRINT)
			printf("%d\t",H[j]);
			#endif
		}
		#if defined(PRINT)
		printf("\n");
		#endif
	}
	
	return H[m-1];
}
