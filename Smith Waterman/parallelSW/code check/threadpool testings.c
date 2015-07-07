#include "threadpool.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void function(int job);
void* destroy_async(void* p);

int main(int argc,char* argv[]){
	int i;
	char* ptr;
	int test;
	pthread_t tid;
	threadpool* p=NULL;
	if(argc!=2){
		perror("Usage: <num of test>\n");
		return -1;
	}
	test=strtoul(argv[1],&ptr,0);
	if(*ptr!='\0')
	{
		perror("Illegal input for <num of test>\n");
		return -1;
	}
	//**************************************************************
	if(test==0){
		fprintf(stdout,"init with negative number of threads\n");
		p=create_threadpool(-20);
		if(p==NULL)
			fprintf(stdout,"create_threadpool failed -20\n");
		return 0;
	}
	//**************************************************************
	if(test==1){
		fprintf(stdout,"init with big number of threads\n");
		p=create_threadpool(203);
		if(p==NULL)
			fprintf(stdout,"create_threadpool failed 203\n");
		//ask shimrit if 200 itself is valid or not
		/*p=create_threadpool(200);
		if(p==NULL)
			fprintf(stdout,"create_threadpool failed 200\n");*/
		return 0;
	}
	//**************************************************************
	if(test==2){
		fprintf(stdout,"init with zero threads\n");	//######### ask shimrit if 0 included or not
		p=create_threadpool(0);
		if(p==NULL)
			fprintf(stdout,"create_threadpool failed 0\n");
		return 0;
	}	
	//**************************************************************
	if(test==3){
		fprintf(stdout,"destroy uninit threadpool\n");
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==4){
		fprintf(stdout,"create and destroy threadpool\n");
		p=create_threadpool(50);
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==5){
		fprintf(stdout,"dispatch with uninit threadpool\n");
		dispatch(p,(dispatch_fn)function,NULL);
		return 0;
	}
	//**************************************************************
	if(test==6){
		fprintf(stdout,"dispatch uninit threadpool with NULL function\n");
		dispatch(p,NULL,NULL);
		return 0;
	}
	//**************************************************************
	if(test==7){
		fprintf(stdout,"dispatch threadpool with NULL function\n");
		p=create_threadpool(13);
		dispatch(p,NULL,(void*)5);
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==8){
		fprintf(stdout,"dispatch threadpool that have been freed\n");
		p=create_threadpool(13);
		destroy_threadpool(p);
		p=NULL;
		dispatch(p,(dispatch_fn)function,NULL);
		return 0;
	}
	//**************************************************************
	if(test==9){
		fprintf(stdout,"dispatch threadpool once with 1 job\n");
		p=create_threadpool(10);
		dispatch(p,(dispatch_fn)function,(void*)5);
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==10){
		fprintf(stdout,"dispatch threadpool once with 20 jobs\n");
		p=create_threadpool(10);
		for(i=0;i<20;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==11){
		fprintf(stdout,"dispatch threadpool 2 times\n");
		p=create_threadpool(10);
		for(i=0;i<20;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		for(i=20;i<35;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		destroy_threadpool(p);
		return 0;
	}
	//**************************************************************
	if(test==12){
		fprintf(stdout,"dispatch threadpool once destroy and dispatch again\n");
		p=create_threadpool(10);
		for(i=0;i<20;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		pthread_create(&tid,NULL,destroy_async,(void*)p);	//####### ask shimrit if we must do that
		for(i=20;i<35;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		pthread_join(tid,NULL);
		return 0;
	}
	//**************************************************************
	if(test==13){
		fprintf(stdout,"dispatch threadpool 2 times\n");
		p=create_threadpool(15);
		for(i=0;i<20;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		sleep(5);
		for(i=20;i<35;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		sleep(3);
		for(i=35;i<45;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		destroy_threadpool(p);
	}
	//**************************************************************
	if(test==14){
		fprintf(stdout,"dispatch to 0 threadpool\n");
		p=create_threadpool(0);
		for(i=0;i<20;i++)
			dispatch(p,(dispatch_fn)function,(void*)i);
		destroy_threadpool(p);
	}
	
	
	/*
	threadpool* p=create_threadpool(threadNum);
	for(i=0;i<jobNum;i++)
		dispatch(p,(dispatch_fn)function,(void*)i);
	destroy_threadpool(p);*/
	return 0;
}

void function(int job){
	int i;
	for(i=0;i<3;i++){
		fprintf(stdout,"%d\n",job);
		sleep(1);	
	}
}

void* destroy_async(void* p){
	fprintf(stdout,"destroy begin\n");
	threadpool* tp=(threadpool*)p;
	destroy_threadpool(tp);
	return NULL;
}
