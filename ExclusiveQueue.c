#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "ExclusiveQueue.h"

void EQ_Init(ExclusiveQueue *pQueue)
{
	// randomN은 여기서 초기화 하지 않고 hw4.c 의 main에서 따로 초기화 할 것임.
	// initialize head and tail
	pQueue -> head = 0; 
	pQueue -> tail = 0;	
	// initialize mutex
	if (pthread_mutex_init(&(pQueue -> mutex), NULL) != 0){
	printf("failed to initialize mutex. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
	// initialize empty by 100 and full by 0
	if (sem_init(&(pQueue -> empty), 0, 100) != 0){
	printf("failed to initialize empty. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
	if (sem_init(&(pQueue -> full), 0, 0) != 0){
	printf("failed to initialize full. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
}	

void EQ_Destroy(ExclusiveQueue *pQueue)
{
	// destroy mutex, empty and full
	if (pthread_mutex_destroy(&(pQueue -> mutex)) != 0){
	printf("failed to destory mutex. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
	if (sem_destroy(&(pQueue -> empty)) != 0){
	printf("failed to destory empty. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
	if (sem_destroy(&(pQueue -> full)) != 0){
	printf("failed to destory full. (file %s, line %d)\n", __FILE__, __LINE__);
	exit(-1);
	}
}

void EQ_Add(ExclusiveQueue *pQueue, int v)
// This function is called by producer
{
	// implement the entry section of producer  (wait for empty and mutex)
	sem_wait(&(pQueue -> empty));
	pthread_mutex_lock(&(pQueue -> mutex));
	// insert v into the queue (critical section)
	// checking full condition is not necessary
	pQueue -> array[pQueue -> head] = v;
	pQueue -> head = ((pQueue -> head) + 1 ) % QueueSize; 
	// implement the exit section of producer (signal mutex and full)
	pthread_mutex_unlock(&(pQueue -> mutex));
	sem_post(&(pQueue -> full));
}

int EQ_Delete(ExclusiveQueue *pQueue)
// This function is called by consumer
{
	int v = 0;
	
	// implement the entry section of consumer  (wait for full and mutex)
	sem_wait(&(pQueue -> full));
	pthread_mutex_lock(&(pQueue -> mutex));	
	// retrieve an item into v and delete it from the queue (critical section)
	// checking empty condition is not necessary
	v = pQueue -> array[pQueue -> tail];
	pQueue -> tail = ((pQueue -> tail) + 1) % QueueSize;
	// implement the exit section of consumer (signal mutex and empty)
	pthread_mutex_unlock(&(pQueue -> mutex));
	sem_post(&(pQueue -> empty));
	
	return v;
}

