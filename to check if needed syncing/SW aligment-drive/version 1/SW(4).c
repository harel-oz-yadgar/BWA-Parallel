//this is the basic Smith-Waterman algorithm with run time of O(n^2*m+n*m^2) and memory of O(n*m) 
#include <stdio.h>

int SW(char* a,char* b,int(*s)(char,char),int(*W)(int)){
	int i,j,n,m;
	int **H;
	
	n=strlen(a);
	m=strlen(b);
	H=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++)
		H[i]=(int*)malloc(m*sizeof(int));
		
	for(i=0;i<n;i++)
		H[i][0]=0;
	for(j=0;j<m;j++)
		H[0][j]=0;
		
	for(i=1;i<n;i++)
		for(j=1;j<m;j++){
			H[i][j]=0;
			H[i][j]=Max(H[i][j],H[i-1][j-1]+s(a[i],b[j]));
			for(k=1;k<=i;k++)
				H[i][j]=Max(H[i][j],H[i-k][j]+W(k));
			for(l=1;l<=j;l++)
				H[i][j]=Max(H[i][j],H[i][j-l]+W(l));
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