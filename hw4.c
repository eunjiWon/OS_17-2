#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

#include "ExclusiveQueue.h"

#define TRUE 1
#define FALSE 0

#define NoConsumer 5				// # of consumer threads
int gThreadContinue = TRUE;

void* ProducerThreadFn(void *param);
void* ConsumerThreadFn(void *param);

int main()
{
	int t = 0;

	// TO DO: initialize random seed	
	ExclusiveQueue queue;
	queue.randomN = 0;
	
	// TO DO: initialize an ExclusiveQueue variable
	EQ_Init(&queue);

	pthread_t producer = 0;
	pthread_t consumer[NoConsumer];
	// TO DO: launch producer and consumer threads
	pthread_create(&producer, NULL, ProducerThreadFn, &queue); 
		
	for(t = 0; t < NoConsumer; t++){
	pthread_create(&consumer[t], NULL, ConsumerThreadFn, &queue);
	}
	
	sleep(10); // 10초 후에 종료된다.

	
	// TO DO: terminate threads by setting gThreadContinue by FALSE
	gThreadContinue = FALSE;
	
	// TO DO: Think and find out why the following code is necessary.
	
	/* 아래의 코드가 하는 역할은 queue의 empty의 값이 1보다 작아질 때,
	 즉 queue에 넣을 수 있는 빈공간이 없을 때 EQ_Delete 를 호출해
	 queue의 빈공간을 만들어 주는 역할을 한다.
	 이 코드가 필요한 이유는 queue의 빈공간이 없으면
	 producer가 queue의 빈공간이 있을 때 까지 기다리기 때문이다. 
	 아래의 코드가 없으면 ProducerFn에서 EQ_Add가 실행될 때	
	 sem_wait 때문에 queue의 빈공간이 없으면 빈공간이 생길 때 까지 
	 계속 기다리게 돼서 ProducerFn 이 종료를 못하게 된다.	
	 그렇게 되면 pthread_join 에서 producer thread가 끝날 때 까지
	 기다리고 있기 때문에 전체 프로그램이 안끝나게 된다. */
	
	int semValue = 0;	
	sem_getvalue(&queue.empty, &semValue);
	while(semValue < 1){
		printf("Releasing producer\n");
		EQ_Delete(&queue);
		sem_getvalue(&queue.empty, &semValue);
	}
	
	
	// TO DO: wait for threads to terminate
	pthread_join(producer, NULL);
	for(t = 0; t < NoConsumer; t++){
	pthread_join(consumer[t], NULL); 
	}
		
	printf("Bye!\n");
	pthread_exit(0);
	// TO DO: destroy the ExclusiveQueue
	EQ_Destroy(&queue);
	free(&queue);
		
	return 0;
}

void* ProducerThreadFn(void *param)
{
	ExclusiveQueue *pQueue = (ExclusiveQueue*) param;

	while(gThreadContinue){
		
		// TO DO: generate a random number between 1 and 100, and add it to the queue.
		pQueue -> randomN = rand() % 100 + 1;
		EQ_Add(pQueue, pQueue -> randomN); 
	}

	printf("Terminating producer.\n");

	return NULL;
}

void* ConsumerThreadFn(void *param)
{
	ExclusiveQueue *pQueue = (ExclusiveQueue*) param;
	int i = 0, sum = 0;
	int v = 0;

	while(gThreadContinue){
		// TO DO: complete this code
					
		// 	retrieve a number v from the queue.
		v = EQ_Delete(pQueue);	
		// 	print the sum of numbers from 1 to v.
		for (i = 1; i <= v; i++){
		sum = sum + i;	
		}
		printf("Sum of numbers from 1 to %d : %d\n", v, sum);

		// 	sleep for 1 second.
		sleep(1);
	}


	printf("Terminating consumer.\n");

	return NULL;
}
