#ifndef	__ExclusiveQueue__
#define	__ExclusiveQueue__

#define QueueSize 100

typedef struct {
	int array[QueueSize];
	int head, tail;
	pthread_mutex_t mutex;
	sem_t empty, full;
	int randomN; // random한 숫자를 만들기 위해 새로운 변수를 struct에 추가
} ExclusiveQueue;

void EQ_Init(ExclusiveQueue *pQueue);
void EQ_Destroy(ExclusiveQueue *pQueue);
void EQ_Add(ExclusiveQueue *pQueue, int v);
int EQ_Delete(ExclusiveQueue *pQueue);

#endif	//	__ExclusiveQueue__

