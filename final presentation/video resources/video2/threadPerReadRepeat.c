#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threadpool.h"
#include <time.h>

#define CHUNK 200
#define BASE_PER_LINE 70
#define MAX_READ_LENGTH 70

#define UP 8
#define LEFT 4
#define DIAGONAL 7
#define STOP 0

#define THREADPOOL_SIZE 4

//#define USE_THREADS

int s(char a, char b){
	return a==b ? 2:-1;
}
int W(int i){
	return -i;
}

void printDNA(char* dna, int length, FILE* stream);
int Max(int a, int b);
int Max3(int a, int b, int c);
int SW(char* a, char* b, int m, int(*s)(char, char), int(*W)(int));
void trackbackSW(char* a, char* b, int m, int(*s)(char, char), int(*W)(int), FILE* stream);
void* processRead(void *args);

char *DNA=NULL;
int difference;
time_t start, end;
double cpu_time_used=0, thread_time=0;

typedef struct{
	int readID, numOfHits;
	char *read;
	int *hitOffsets;
	FILE *resultsFile;
}processReadData;

int main(int argc, char *argv[]){
	
	FILE *DNAfile, *readsFile, *hitsFile, *resultsFile;
	int readedSize, DNAlength;
	int readID, readLen, i, numOfHits, hit, hitScore, bestHitScore, bestHitOffset;
	char DNAbuffer[CHUNK];
	char readBuf[MAX_READ_LENGTH];
	char hitsBuf[20];
	int *hitsOffsets;
	threadpool *tp;
	processReadData *arguments;
	int timesRepeated=0;
	int totalHits=0;
	
	#if defined(USE_THREADS)
		printf("Using %d threads\n",THREADPOOL_SIZE);
	#else
		printf("Does not using threads\n");
	#endif
	printf("finiding optimal matchs for %d reads\n",500*20);
	
	if(argc!=6){
		printf("Usage: ./refine.out <dna File> <reads File> <hits File> <difference> <results File>\n");
		return 1;
	}
	
	if((tp=create_threadpool(THREADPOOL_SIZE))==NULL){
		printf("Error: could not open threadpool\n");
		return 1;
	}
	
	DNAfile=fopen(argv[1], "r");
	if(DNAfile==NULL){
		printf("Error: cannot open DNA file\n");
		destroy_threadpool(tp);
		return 1;
	}
	
	readsFile=fopen(argv[2], "r");
	if(readsFile==NULL){
		printf("Error: cannot open reads file\n");
		destroy_threadpool(tp);
		fclose(DNAfile);
		return 1;
	}
	
	hitsFile=fopen(argv[3], "r");
	if(hitsFile==NULL){
		printf("Error: cannot open hits file\n");
		destroy_threadpool(tp);
		fclose(DNAfile);
		fclose(readsFile);
		return 1;
	}
	
	difference=strtoul(argv[4], NULL, 0);
	
	resultsFile=fopen(argv[5], "w+");
	if(resultsFile==NULL){
		printf("Error: cannot create results file\n");
		destroy_threadpool(tp);
		fclose(DNAfile);
		fclose(readsFile);
		fclose(hitsFile);
		return 1;
	}
	


 	//read unknown length DNA from a file
 	DNAlength=0;
 	readedSize=0;
 	do{
 		fgets(DNAbuffer, CHUNK, DNAfile);
 		readedSize=strlen(DNAbuffer);
 		DNAlength+=readedSize;
 		DNA=realloc(DNA, DNAlength+1);
 		strcat(DNA, DNAbuffer);
 	}while(readedSize==CHUNK-1 && DNAbuffer[CHUNK-2]!='\n');
 	
	//printDNA(dna, length, stdout);
	
	
	start=time(NULL);
	for(timesRepeated=0;timesRepeated<500;timesRepeated++){
		rewind(readsFile);
		rewind(hitsFile);

		//read unknown nomber of reads from a file
		readID=0;
		while(fgets(readBuf, MAX_READ_LENGTH, readsFile)!=NULL){	//for each read
			readLen=strlen(readBuf);
			for(i=0;i<readLen;i++)
				if(readBuf[i]=='\n')
					readBuf[i]='\0';
		
			if(fgets(hitsBuf, 20, hitsFile)==NULL){
				printf("Error: hits file corrupted\n");
				destroy_threadpool(tp);
				free(DNA);
				fclose(DNAfile);
				fclose(readsFile);
				fclose(hitsFile);
				fclose(resultsFile);
				return 1;	
			}
			numOfHits=strtoul(hitsBuf, NULL, 0);
			totalHits+=numOfHits;
			
			if(numOfHits==0){
				arguments=(processReadData*)malloc(sizeof(processReadData));
				arguments->readID=readID;
				arguments->numOfHits=0;
				arguments->read=strdup(readBuf);
				arguments->hitOffsets=NULL;
				arguments->resultsFile=resultsFile;
				
				#if defined(USE_THREADS)
					dispatch(tp,(dispatch_fn)processRead,(void*)arguments);
				#else
					processRead(arguments);
				#endif
			}
			else{
				hitsOffsets=(int*)malloc(numOfHits*sizeof(int));
				for(hit=0;hit<numOfHits;hit++){
					fgets(hitsBuf, 20, hitsFile);
					hitsOffsets[hit]=strtoul(hitsBuf, NULL, 0);
				}

				arguments=(processReadData*)malloc(sizeof(processReadData));
				arguments->readID=readID;
				arguments->numOfHits=numOfHits;
				arguments->read=strdup(readBuf);
				arguments->hitOffsets=hitsOffsets;
				arguments->resultsFile=resultsFile;
			
				#if defined(USE_THREADS) 
					dispatch(tp,(dispatch_fn)processRead,(void*)arguments);
				#else
					processRead(arguments);
				#endif
			}
			readID++;
		}	
	}
	#if defined(USE_THREADS)
		destroy_threadpool(tp);
	#endif
	
	
	
	end=time(NULL);
	cpu_time_used+=(double)(end-start);
	printf("Total run-time: %lf\n",cpu_time_used);
	printf("number of hits the been processed: %d\n",totalHits);

	
	fclose(DNAfile);
	fclose(readsFile);
	fclose(hitsFile);
	fclose(resultsFile);
	
	return 0;
}

//print the DNA
void printDNA(char* dna, int length, FILE* stream){
	int i, offset;
	for(i=0, offset=0;offset<length;i++){
		offset=i*BASE_PER_LINE;
		if(offset+BASE_PER_LINE>length)
			break;
		fwrite(&(dna[offset]), sizeof(char), BASE_PER_LINE, stream);
		fputs("\n", stream);
	}
	fprintf(stream, "%s\n", &(dna[offset]));
}

//Smith-Waterman aligment related functions
int Max(int a, int b){
	return a>b ? a:b;
}

int Max3(int a, int b, int c){
	if(b>c)
		return a>b ? a:b;
	return a>c ? a:c;
}

int SW(char* a, char* b, int m, int(*s)(char, char), int(*W)(int)){
	int i, j, n, Hdiagonal, tmp, maxIpos, maxJpos, maxValue=-1;

	n=strlen(a)+1;	//O(n)
	m=m+1;//strlen(b)+1;	//O(m)
	
	int H[m];
	int Ix[m];
	int Iy[m];
	
	for(j=0;j<m;j++){	//O(m)
		H[j]=0;
		Ix[j]=0;
		Iy[j]=0;
	}
	
	for(i=1;i<n;i++){	//O(n*m)
		Hdiagonal=H[0];
		for(j=1;j<m;j++){	//O(m)
			Ix[j]=Max(H[j]+W(1), Ix[j]+W(1));
			Iy[j]=Max(H[j-1]+W(1), Iy[j-1]+W(1));
			tmp=H[j];
			H[j]=Max3(Hdiagonal+s(a[i-1], b[j-1]), Ix[j], Iy[j]);
			H[j]=Max(H[j], 0);
			Hdiagonal=tmp;
			if(H[j]>=maxValue){
				maxValue=H[j];
				maxIpos=i-1;
				maxJpos=j-1;
			}
		}
	}
	return maxValue;
}

void mirrorString(char str[], int length){
	int i;
	char tmp;
	for(i=0;i<length/2;i++){
		tmp=str[i];
		str[i]=str[length-i-1];
		str[length-i-1]=tmp;
	}
}

void trackbackSW(char* a, char* b, int m, int(*s)(char, char), int(*W)(int), FILE* stream){
	int i, j, n, len, maxIpos, maxJpos, maxValue=-1;

	n=strlen(a)+1;	//O(n)
	m=m+1;//strlen(b)+1;	//O(m)
	
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
			
			
			H[i][j]=H[i-1][j-1]+s(a[i-1], b[j-1]);
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
				H[i][j]=Max(H[i][j], 0);
				T[i][j]=STOP;
			}
			
			if(H[i][j]>maxValue){
				maxValue=H[i][j];
				maxIpos=i;
				maxJpos=j;
			}
			
		}

	#if defined(PRINT) 
	printf("H -\t");
	for(j=0;j<m;j++)
		printf("%c\t", b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("- ");
		else
			printf("%c: ", a[i-1]);
		for(j=0;j<m;j++)
			printf("%d\t", H[i][j]);
		printf("\n");
	}
	
	printf("\n\nT -\t");
	for(j=0;j<m;j++)
		printf("%c\t", b[j]);
	printf("\n");
	for(i=0;i<n;i++){
		if(i==0)
			printf("- ");
		else
			printf("%c: ", a[i-1]);
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
	i=maxIpos;
	j=maxJpos;
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
			if(a[i-1]==b[j-1])
				alignment[1][len]='|';
			else
				alignment[1][len]='X';
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
		mirrorString(alignment[i], len);
	fprintf(stream,"%s\n%s\n%s\n", alignment[2], alignment[1], alignment[0]);
}

void* processRead(void *args){
	int readID, hit, numOfHits, hitScore, bestHitScore, bestHitOffset;
	char *read;
	int *hitsOffsets;
	FILE *resultsFile;
	
	processReadData *arguments;
	arguments=(processReadData*)args;
	readID=arguments->readID;
	numOfHits=arguments->numOfHits;
	read=arguments->read;
	hitsOffsets=arguments->hitOffsets;
	resultsFile=arguments->resultsFile;
	
	free(arguments);
	
	if(numOfHits==0){
		fprintf(resultsFile,"Read id: %d\n%s\nLocation: NONE\n",readID,read);
		fprintf(resultsFile,"Score: NONE\n");
		
		free(read);
		free(hitsOffsets);
		return;
	}

	for(hit=0;hit<numOfHits;hit++){	
		hitScore=SW(read, &DNA[hitsOffsets[hit]], strlen(read)+difference, s, W);
		if(hit==0 || hitScore>bestHitScore){
			bestHitScore=hitScore;
			bestHitOffset=hitsOffsets[hit];
		}
	}
	
	/*fprintf(resultsFile,"Read id: %d\n%s\nhit offset: %d\n",readID,read, bestHitOffset);
	trackbackSW(read, &DNA[bestHitOffset], strlen(read)+difference, s, W, resultsFile);
	fprintf(resultsFile,"Score: %d\n",bestHitScore);*/

	free(read);
	free(hitsOffsets);
}
