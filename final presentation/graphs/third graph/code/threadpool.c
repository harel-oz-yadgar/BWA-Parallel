#include "threadpool.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

threadpool* create_threadpool(int num_threads_in_pool){
	if(num_threads_in_pool<=0 || MAXT_IN_POOL<num_threads_in_pool)	//if the input is invalid return NULL
		return NULL;
	threadpool* tp=(threadpool*)malloc(sizeof(threadpool));	//allocate the threadpool memory
	if(tp==NULL){
		perror("error: malloc() ");
		return NULL;
	}

	//initialize the parameters
	tp->num_threads=num_threads_in_pool;	
	tp->qsize=0;
	tp->qhead=NULL;
	tp->qtail=NULL;
	tp->shutdown=0;
	tp->dont_accept=0;
	
	if(pthread_mutex_init(&tp->qlock,NULL)!=0){	//initialize the mutex
		perror("error: pthread_mutex_init() ");
		free(tp);
		return NULL; 
	}
	if(pthread_cond_init(&tp->q_not_empty,NULL)!=0){	//initialize the cond_v
		perror("error: pthread_cond_init() ");
		pthread_mutex_destroy(&tp->qlock);
		free(tp);
		return NULL;
	}
	if(pthread_cond_init(&tp->q_empty,NULL)!=0){	//initialize the cond_v
		perror("error: pthread_cond_init() ");
		pthread_mutex_destroy(&tp->qlock);
		pthread_cond_destroy(&tp->q_not_empty);		
		free(tp);
		return NULL;
	}
	
	tp->threads=(pthread_t*)malloc(tp->num_threads*sizeof(pthread_t));	//allocate the memory for the threads tids
	if(tp->threads==NULL){
		perror("error: malloc() ");
		pthread_mutex_destroy(&tp->qlock);
		pthread_cond_destroy(&tp->q_not_empty);	
		pthread_cond_destroy(&tp->q_empty);
		free(tp);
		return NULL;
	}
	
	int i,j;
	for(i=0;i<tp->num_threads;i++)	//create num_threads threads with do_work as the threads
		if(pthread_create(tp->threads+i,NULL,do_work,(void*)tp)!=0){
			perror("error: pthread_create() ");
			for(j=0;j<i;j++)
				pthread_cancel(tp->threads[j]);
			free(tp->threads);
			pthread_mutex_destroy(&tp->qlock);
			pthread_cond_destroy(&tp->q_not_empty);	
			pthread_cond_destroy(&tp->q_empty);	
			free(tp);
			return NULL;
		}
	return tp;	//return the threadpool
}

void dispatch(threadpool* from_me, dispatch_fn dispatch_to_here, void *arg){
	if(from_me==NULL || from_me->threads==NULL || dispatch_to_here==NULL)
		return;
	pthread_mutex_lock(&from_me->qlock);	//acquire the lock
	if(from_me->dont_accept==1){	//if destruction has begun free the lock and return
		pthread_mutex_unlock(&from_me->qlock);
		return;
	}
	//allocate and initialize new work
	work_t* newWork=(work_t*)malloc(sizeof(work_t));
	if(newWork==NULL){
		perror("error: malloc() ");
		pthread_mutex_unlock(&from_me->qlock);
		return;
	}
	newWork->routine=dispatch_to_here;
	newWork->arg=arg;
	newWork->next=NULL;

	if(from_me->qsize==0){	//if the queue is empty set the head and tail of the queue to 'newWork'
		from_me->qhead=newWork;
		from_me->qtail=newWork;
	}
	else{	//the queue isn't empty than add 'newWork' to the tail of the queue
		from_me->qtail->next=newWork;
		from_me->qtail=newWork;
	}
	from_me->qsize=from_me->qsize+1;	//add 1 to the size of the queue
	pthread_cond_signal(&from_me->q_empty);	//signal the threads that the queue isn't empty
	pthread_mutex_unlock(&from_me->qlock);	//free the lock
}

void* do_work(void* p){
	threadpool* tp=(threadpool*)p;
	if(tp==NULL || tp->threads==NULL)	//if the threadpool or the threads tids are NULL return NULL
		return NULL;
	work_t* work;
	while(1){	//endless loop
		pthread_mutex_lock(&tp->qlock);	//acquire the lock
		if(tp->shutdown==1){	//if destroy begun free the lock and return NULL
			pthread_mutex_unlock(&tp->qlock);	
			return NULL;
		}
		while(tp->qsize==0){	//if the the job queue size is 0 then wait on q_empty
			pthread_cond_wait(&tp->q_empty,&tp->qlock);
			if(tp->shutdown==1){	//if destroy begun free the lock and return NULL
				pthread_mutex_unlock(&tp->qlock);
				return NULL;
			}
		}
		//take out job from the queue
		work=tp->qhead;
		tp->qsize=tp->qsize-1;
		if(tp->qsize==0){
			tp->qhead=NULL;
			tp->qtail=NULL;
			if(tp->dont_accept==1)	//if the queue is empty and destroy is waiting for the queue to become empty signal q_not_empty
				pthread_cond_signal(&tp->q_not_empty);
		}
		else
			tp->qhead=tp->qhead->next;
		
		pthread_mutex_unlock(&tp->qlock);	//acquire the lock
		work->routine(work->arg);	//run the job routine
		free(work);
	}	
	return NULL;
}

void destroy_threadpool(threadpool* destroyme){
	if(destroyme==NULL)
		return;
	pthread_mutex_lock(&destroyme->qlock);	//acquire the lock
	destroyme->dont_accept=1;	//don't accept any more routines into the queue
	
	if(destroyme->qsize!=0)	//if the queue isn't empty wait on q_not_empty
		pthread_cond_wait(&destroyme->q_not_empty,&destroyme->qlock);
	
	destroyme->shutdown=1;	//
	
	pthread_mutex_unlock(&destroyme->qlock);	//free the lock
	pthread_cond_broadcast(&destroyme->q_empty);	//signal all waiting thread that they need to wake up
	int i;
	for(i=0;i<destroyme->num_threads;i++)	//join all the threads
		pthread_join(destroyme->threads[i],NULL);
	pthread_mutex_destroy(&destroyme->qlock);	//destroy the lock
	pthread_cond_destroy(&destroyme->q_not_empty);	//destroy the cond_v
	pthread_cond_destroy(&destroyme->q_empty);	//destroy the cond_v
	free(destroyme->threads);	//free the threads tid memory
	destroyme->threads=NULL;
	free(destroyme);	//free the threadpool
}
