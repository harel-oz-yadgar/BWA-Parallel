#include <stdio.h>
#include <pthread.h>

void* readHandler(void* p);
int nextToWrite;
pthread_mutex_t lock; 
pthread_cond_t cv; 

#define N 10


int main(){
	int i;
	pthread_t tids[N]; 
	nextToWrite=0;

	pthread_mutex_init(&lock, NULL); 
	pthread_cond_init(&cv, NULL); 
	
	for(i=N-1;i>=0;i--) 
		pthread_create(tids+i,NULL,readHandler,(void*)i); 
	
	pthread_cond_signal(&cv);
	//for(i=0;i<10;i++) 
	//	pthread_join(tids[i],NULL); 
}


void* readHandler(void* p){
	int tid=(int)p;
	printf("%d created\n",tid);
	
	//do work
	
	pthread_mutex_lock(&lock); 
	while(tid!=nextToWrite)
		pthread_cond_wait(&cv,&lock); 
	nextToWrite++;
	printf("tid: %d\n",tid);
	pthread_cond_broadcast(&cv);
	pthread_mutex_unlock(&lock);
}
