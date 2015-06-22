#include "SW.h"
#include <stdio.h>
#include <string.h>

#define PRINT

#define UP 8
#define LEFT 4
#define DIAGONAL 7
#define STOP 0


int Max(int a,int b){
	return a>b ? a:b;
}

int Max3(int a,int b,int c){
	if(b>c)
		return a>b ? a:b;
	return a>c ? a:c;
}

void mirrorString(char str[],int length){
	int i;
	char tmp;
	for(i=0;i<length/2;i++){
		tmp=str[i];
		str[i]=str[length-i-1];
		str[length-i-1]=tmp;
	}
}

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m,len;

	n=strlen(a)+1;	//O(n)
	m=strlen(b)+1;	//O(m)
	
	int H[n][m];
	int Ix[n][m];
	int Iy[n][m];
	int T[n][m];
	char alignment[3][m+n+1];
		
	for(i=0;i<n;i++){	//O(n)
		H[i][0]=0;
		Ix[i][0]=0;
		Iy[i][0]=0;
		T[i][0]=STOP;
	}
	for(j=0;j<m;j++){	//O(m)
		H[0][j]=0;
		Ix[0][j]=0;
		Iy[0][j]=0;
		T[0][j]=STOP;
	}
		
	for(i=1;i<n;i++)	//O(n*m)
		for(j=1;j<m;j++){	//O(m)
			Ix[i][j]=Max(H[i-1][j]+W(1), Ix[i-1][j]+W(1));
			Iy[i][j]=Max(H[i][j-1]+W(1), Iy[i][j-1]+W(1));
			
			
			H[i][j]=H[i-1][j-1]+s(a[i-1],b[j-1]);
			T[i][j]=DIAGONAL;
			if(H[i][j]<Ix[i][j]){
				H[i][j]=Ix[i][j];
				T[i][j]=UP;
			}
			if(H[i][j]<Iy[i][j]){
				H[i][j]=Iy[i][j];
				T[i][j]=LEFT;
			}			
			if(H[i][j]<0){
				H[i][j]=Max(H[i][j],0);
				T[i][j]=STOP;
			}
			
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
	
	printf("\n\nT  -\t");
	for(j=0;j<m;j++)
		printf("%c\t",b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("-  ");
		else
			printf("%c: ",a[i-1]);
		for(j=0;j<m;j++){
			if(T[i][j]==STOP)
				printf("*\t");
			if(T[i][j]==UP)
				printf("^\t");
			if(T[i][j]==LEFT)
				printf("<\t");
			if(T[i][j]==DIAGONAL)
				printf("\\\t");
			
		}
		printf("\n");
	}
	#endif
	
	len=0;
	i=n-1;
	j=m-1;
	while(T[i][j]!=STOP){
		if(T[i][j]==UP){
			alignment[0][len]=a[i-1];
			alignment[1][len]=' ';
			alignment[2][len]='-';
			i--;
			len++;
		}
		else if(T[i][j]==LEFT){
			alignment[0][len]='-';
			alignment[1][len]=' ';
			alignment[2][len]=b[j-1];
			j--;
			len++;
			continue;
		}
		else{//T[i][j]==DIAGONAL
			alignment[0][len]=a[i-1];
			alignment[1][len]='|';
			alignment[2][len]=b[j-1];
			i--;
			j--;
			len++;
		}
	}
	alignment[0][len]='\0';
	alignment[1][len]='\0';
	alignment[2][len]='\0';
	for(i=0;i<3;i++)
		mirrorString(alignment[i],len);
	printf("dna:  %s\n      %s\nread: %s\n",alignment[2],alignment[1],alignment[0]);
	
	return H[n-1][m-1];
}
