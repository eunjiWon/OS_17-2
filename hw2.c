#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#define BUFFER_SIZE 10
#define MAX_LEN 256

typedef struct {
char data[BUFFER_SIZE][MAX_LEN];
int in, out;
}Buffer;

void producer(Buffer *buffer){

int count = 0;
char *line = (char*)malloc(sizeof(Buffer)); //사용자 입력을 받을 변수  
int turn;
_Bool flag[2];

/* printf("Input a string:");
   gets(line);
   위와 같이 코드를 짰더니 Consumer(child process)가 gets문 전에 실행되어
   "Input a string: " 다음에 바로 "Consumer deleted --- " 가 출력되었습니다.
   그래서 먼저 입력을 받은 후에 "Input a string: ~~~ " 이라는 문장이 한 번에 출력될 수 있게 하였습니다.    */
  
do{
printf("Input a string: %s\n", gets(line));
//entry section
flag[0] = true;
turn = 1;
while(flag[1] && turn == 1);
//critical section
while (((buffer -> in + 1) % BUFFER_SIZE) == buffer -> out);
strcpy (buffer -> data[buffer -> in] , line);
buffer -> in = (buffer -> in + 1) % BUFFER_SIZE;
//exit section
flag[0] = false;
//remainder section
printf("Producer inserting \"%s\" \n", line);  
count++;
}while(count < 100);

free(line);
}

void consumer(Buffer *buffer){

int turn;
_Bool flag[2];
int count = 0;
char *line = (char*)malloc(sizeof(Buffer)); // 삭제될 string을 담아놓을 변수  
do{
//entry section
flag[1] = true;
turn = 0;
while(flag[0] && turn == 0 );
//critical section
while (buffer -> in == buffer -> out);
strcpy(line, buffer -> data[buffer -> out]);
buffer -> out = (buffer -> out + 1) % BUFFER_SIZE;
printf("\t Consumer deleted \"%s\" \n", line);
//exit section
flag[1] = false;
//remainder section
count++;
sleep(2); 
}while(count < 100);

free(line);
}

int main (){

int pid;
int segment_id;
const int size = sizeof(Buffer); 
//공유 메모리 세그먼트 할당
segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
//shmget() 실행하고 fork()를 호출해야 segment_id가 child proces에게도 그대로 복사되게 된다. 
pid = fork();

if(pid < 0){
fprintf(stderr, "Fork Failed\n");
return -1;
}
//child process
else if(pid == 0){ 
Buffer *buffer_child = (Buffer*)malloc(sizeof(Buffer));
//공유 메모리 세그먼트 부착
buffer_child = (Buffer*)shmat(segment_id, NULL, 0);
consumer(buffer_child);
//공유 메모리 세그먼트 분리
shmdt(buffer_child);
}
//parent process
else{ 
Buffer *buffer_parent = (Buffer*)malloc(sizeof(Buffer));
//공유 메모리 세그먼트 부착
buffer_parent = (Buffer*)shmat(segment_id, NULL, 0);
//버퍼 초기화
buffer_parent -> in = 0;
buffer_parent -> out = 0;
producer(buffer_parent);
//공유 메모리세그먼트 분리
shmdt(buffer_parent);
//공유 메모리 세그먼트 제게
shmctl(segment_id, IPC_RMID, NULL);
}

return 0;
}

