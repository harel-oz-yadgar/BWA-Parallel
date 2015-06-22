#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BASE_PER_LINE 70

char generateBase();
char generateBaseOtherThan(char c);
char* generateDNA(int length);
char** generateReads(int numOfReads, int length);
char* generateDNAFromReads(char** reads, int numOfReads, int covarage, int diffPerRead);

void addDeletion(char* read, int k, int m, int n);
char* addInsertion(char* read, int k, int m, int n);
void addMismatch(char* read, int k, int m, int n);

void freeDNA(char* dna);
void freeReads(char** reads);

void printDNA(char* dna, int length, FILE* stream);
void printReads(char** reads, FILE* stream);


int main(int argc, char *argv[]){
	int dnaLength, numOfReads, readLength, covarage;
	char* dna;
	char** reads;
	int i;
	FILE *readsFile, *dnaFile;
	srand(time(NULL));
	printf("\n");
	printf("%d\n",argc);
	if(argc==1){
		readsFile=stdout;
		dnaFile=stdout;
	}
	else if(argc==3){
		dnaFile=fopen(argv[1],"w+");
		readsFile=fopen(argv[2],"w+");
	}	
	else if(argc!=3){
		printf("Usage: ./<exec> <dna file> <reads file>\n");
		return 1;
	}
	
	
	dnaLength=100000;
	numOfReads=5;
	readLength=10;
	covarage=10;


	reads=generateReads(numOfReads,readLength);
	printf("-------------------reads-------------------\n");
	printReads(reads,readsFile);
	
	
	dna=generateDNAFromReads(reads,numOfReads,covarage,0);
	
	printf("--------------------dna--------------------\n");
	printf("%d\n",strlen(dna));
	printDNA(dna,strlen(dna),dnaFile);
	
	freeReads(reads);
	freeDNA(dna);
	

	printf("\n");
	return 0;
}

char* generateDNA(int length){
	int i,random;
	
	char* dna=(char*)malloc((length+1)*sizeof(char));
	if(dna==NULL){
		printf("Error: cannot allocate memory\n");
		return NULL;
	}
	
	for(i=0;i<length;i++)
		dna[i]=generateBase();
	dna[i]='\0';
	return dna;
}

char** generateReads(int numOfReads, int length){
	int i,j,random;
	char** reads=(char**)malloc((numOfReads+1)*sizeof(char*));
	if(reads==NULL){
		printf("Error: cannot allocate memory\n");
		return NULL;
	}
	for(i=0;i<numOfReads;i++){
		reads[i]=(char*)malloc((length+1)*sizeof(char));
		if(reads[i]==NULL){
			freeReads(reads);
			printf("Error: cannot allocate memory\n");
			return NULL;
		}
		for(j=0;j<length;j++)
			reads[i][j]=generateBase();
		reads[i][j]='\0';
	}
	reads[i]=NULL;
	return reads;
}

char* generateDNAFromReads(char** reads, int numOfReads, int covarage, int diffPerRead){
	int i,index;
	char *read,*dna;
	char **readsForDNA=(char**)malloc((numOfReads*covarage+1)*sizeof(char*));
	if(readsForDNA==NULL){
		printf("Error: cannot allocate memory\n");
		return NULL;	
	}
	
	for(i=0;i<numOfReads*covarage;i++){
		index=rand()%numOfReads;	
		read=strdup(reads[index]);
		
		addDeletion(read,2,1,2);
		addMismatch(read,5,1,2);
		read=addInsertion(read,2,1,2);
		
		readsForDNA[i]=read;
	}
	readsForDNA[i]=NULL;
	
	int DNAlength=0;
	for(i=0;i<numOfReads*covarage;i++)
		DNAlength+=strlen(readsForDNA[i]);
		
	dna=(char*)malloc((DNAlength+1)*sizeof(char));
	if(dna==NULL){
		printf("Error: cannot allocate memory\n");
		return NULL;	
	}
	dna[0]='\0';
	
	for(i=0;i<numOfReads*covarage;i++)
		strcat(dna,readsForDNA[i]);
		
	//printf("-------------------gnerated dna-------------------\n");
	//printReads(readsForDNA,stdout);
	freeReads(readsForDNA);
	return dna;
}

//have the chance to delete <k> bases with probability of <m>/<n> each
void addDeletion(char* read, int k, int m, int n){
	int length,i,index;
	
	if(read==NULL)
		return;
		
	length=strlen(read);
	for(i=0;i<k;i++)
		if(rand()%n<m){
			index=rand()%length;
			for(;index<length;index++)
				read[index]=read[index+1];
			length=strlen(read);
		}
}

//have the chance to insert <k> bases with probability of <m>/<n> each
char* addInsertion(char* read, int k, int m, int n){
	int length,i,index,j;
	
	if(read==NULL)
		return NULL;
		
	length=strlen(read);
	read=(char*)realloc(read,(length+k+1)*sizeof(char));
	for(i=0;i<k;i++)
		if(rand()%n<m){
			index=rand()%(length+1);
			for(j=length+1;j>index;j--)
				read[j]=read[j-1];
			read[index]=generateBase();
			length++;
		}
	return read;
}

//have the chance to add <k> mismatch bases with probability of <m>/<n> each
void addMismatch(char* read, int k, int m, int n){
	int length,i,index;
	
	if(read==NULL)
		return;
		
	length=strlen(read);
	for(i=0;i<k;i++)
		if(rand()%n<m){
			index=rand()%length;
			read[index]=generateBaseOtherThan(read[index]);
		}
}

//free the DNA data string
void freeDNA(char* dna){
	free(dna);
}

//free the reads data
void freeReads(char** reads){
	int i;
	if(reads==NULL)
		return;
	
	for(i=0;reads[i]!=NULL;i++)
		free(reads[i]);
	free(reads);
}

//random generator of A/G/T/C
char generateBase(){
	int random=rand()%4;
	if(random==0)
		return 'A';
	if(random==1)
		return 'C';
	if(random==2)
		return 'G';
	return 'T';
}

//random generator a base other than the one inserted
char generateBaseOtherThan(char c){
	int i;
	char tmp;
	char options[]={'A','C','G','T'};
	for(i=0;i<3;i++)
		if(options[i]==c){
			tmp=options[i];
			options[i]=options[i+1];
			options[i+1]=tmp;
		}
	return options[rand()%3];
}

//print the DNA
void printDNA(char* dna, int length, FILE* stream){
	int i,offset;
	for(i=0,offset=0;offset<length;i++){
		offset=i*BASE_PER_LINE;
		if(offset+BASE_PER_LINE>length)
			break;
		fwrite(&(dna[offset]),sizeof(char),BASE_PER_LINE,stream);
		//fputs("\n",stream);
	}
	fprintf(stream,"%s\n",&(dna[offset]));
}

//print the diffrente reads
void printReads(char** reads, FILE* stream){
	int i;
	for(i=0;reads[i]!=NULL;i++){
		fputs(reads[i],stream);
		fputs("\n",stream);
	}
}
