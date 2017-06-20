#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int gThreadContinue = TRUE; //global var to control threads
pthread_mutex_t mutex; // mutex for display routines

void DrawBar(int x, int y, int len, char c);
void PrintXY(int x, int y, const char *format, ...);
void gotoxy(int x, int y);
void clrscr();

typedef struct {
int x,y;
int dx;
int len;
int interval;
} BarInfo;

void* MoveBarThreadFn(void *vpParam);

int main(int argc, char *argv[]){

char ch = NULL; //enter key를 저장할 변수
srand(time(NULL));
int noThread = 5; // default
int t = 0; 
int ret = 0; 
pthread_t *tid = NULL; 
BarInfo *barInfo = NULL; 

if(argc > 1){
noThread = atoi(argv[1]);
if(noThread == 0){
printf("Usage: %s <# of thread>\n", argv[0]);
return -1;
}
}

clrscr();
ret = pthread_mutex_init(&mutex, NULL);
if(ret != 0){
printf("failed to initialize mutex. (file %s, line %d)\n", __FILE__, __LINE__);
exit(-1);
}

// TO DO: allcate tid and barInfo
tid = (pthread_t*)malloc(sizeof(pthread_t) * noThread);
if (tid == NULL){
printf("failed to allocate tid.\n");
return 1;
}

barInfo = (BarInfo*)malloc(sizeof(BarInfo) * noThread);
if (barInfo == NULL){
printf("failed to allocate barInfo.\n");
return 1;
}

for (t = 0; t < noThread; t++){
// TO DO: initialize barInfo[t]
barInfo[t].x = 1;
barInfo[t].y = (SCREEN_HEIGHT - 1) / noThread * t + 2;
barInfo[t].dx = 1;
barInfo[t].len  = rand() % 6 + 5;
barInfo[t].interval = (rand() % 100 + 1) * 1000;
// TO DO: launch a thread to animate t-th bar
pthread_create(&tid[t], NULL, MoveBarThreadFn, &barInfo[t]);
}
// Wait for Enter key 
do{
ch = getchar();
}while(ch != '\n');
// Terminate threads by assigning zero to gThreadContinue
gThreadContinue = 0;
// Wait for all children threads
for (t = 0; t < noThread; t++){
pthread_join(tid[t], NULL); 
}
//Clear screen
clrscr();
//Print a good-bye message
printf("Good bye!\n");
//Deallocate memory
pthread_exit(0); 
free(tid);
free(barInfo);

return 0;
}

void* MoveBarThreadFn(void *vpParam){
// TO DO: fill this function
printf("Press Enter to finish!\n");
BarInfo *barInfo = (BarInfo*)vpParam;
// Repeat until gThreadContinue becomes zero
while(gThreadContinue != 0){
DrawBar(barInfo -> x, barInfo -> y, barInfo -> len, ' ');
barInfo -> x += barInfo -> dx;
if ((barInfo -> x + barInfo -> dx ) < 1 || (barInfo -> x + barInfo -> dx + barInfo -> len > SCREEN_WIDTH)){
barInfo -> dx = (barInfo -> dx) * -1;
}
DrawBar(barInfo -> x, barInfo -> y, barInfo -> len, '=');
usleep(barInfo -> interval);
}
}

void DrawBar(int x, int y, int len, char c){
pthread_mutex_lock(&mutex);

gotoxy(x, y);
for(int i = 0; i < len; i++)
putchar(c);

gotoxy(1, 1);

fflush(stdout);

pthread_mutex_unlock(&mutex);
}

void PrintXY(int x, int y, const char *format, ...){

va_list vl;

pthread_mutex_lock(&mutex);

gotoxy(x, y);

va_start(vl, format);
vprintf(format, vl);
va_end(vl);

fflush(stdout);

pthread_mutex_unlock(&mutex);
}

void gotoxy(int x, int y){
printf("%c[%d;%df", 0x1B, y, x);
}

void clrscr(){
system("clear");
gotoxy(1, 1);
}
























