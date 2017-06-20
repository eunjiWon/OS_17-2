/*
*Author: Won Eunji, 21500420@handong.edu
*Written: 2017/3/18
*program name: hw1_3
*
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
int pid;
pid = fork(); // create new process

if(pid < 0){ // fork failed
fprintf(stderr, "Fork Failed");
}

if(pid == 0){ // this is child process
execlp("./hello", "hello", NULL);
}

else{ // this is parent process
wait(NULL); // parent process will wait for child process completion
printf("This is hw1_3.\n");
}

return 0;
}

/*
3
When wait() is removed, the execution result is as follows.
This is hw1_3.
Hello, World!

when wait() is called, the result is:
Hello, World!
This is hw1_3.

When the parent process calls wait(), it pauses itself and waits for the child process to terminate.
When the child process finishes, the parent process resumes.
If the parent process does not call wait, it will continue to run in the background without waiting for the child process.
So, when there is no wait(), the parent process first prints "This is hw1_3." And then the child process prints "Hello, World!" 
*/















