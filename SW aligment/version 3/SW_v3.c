//Smith-Waterman algorithm with run time O(n*m) and memory O(3*2*m)=O(6*m)
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

void swap(int** first,int** second){
	int* tmp=*first;
	*first=*second;
	*second=tmp;
}

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m;
	int *Hfirst,*Hsecond,*Ixfirst,*Ixsecond,*Iyfirst,*Iysecond;

	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[2][m];
	int Ix[2][m];
	int Iy[2][m];
		
	for(i=0;i<2;i++){
		H[i][0]=0;
		Ix[i][0]=0;
		Iy[i][0]=0;
	}
	for(j=0;j<m;j++){	//O(m)
		H[0][j]=0;
		Ix[0][j]=0;
		Iy[0][j]=0;
	}
	Hfirst=H[0];
	Hsecond=H[1];
	Ixfirst=Ix[0];
	Ixsecond=Ix[1];
	Iyfirst=Iy[0];
	Iysecond=Iy[1];
	
	
	for(i=1;i<n;i++){	//O(n*m)
		for(j=1;j<m;j++){	//O(m)
			Ixsecond[j]=Max(Hfirst[j]+W(1), Ixfirst[j]+W(1));
			Iysecond[j]=Max(Hsecond[j-1]+W(1), Iysecond[j-1]+W(1));
			Hsecond[j]=Max3(Hfirst[j-1]+s(a[i-1],b[j-1]), Ixsecond[j], Iysecond[j]);
		}
		swap(&Hfirst,&Hsecond);
		swap(&Ixfirst,&Ixsecond);
		swap(&Iyfirst,&Iysecond);
	}
	
	return Hfirst[m-1];
}
