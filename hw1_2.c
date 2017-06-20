/*
*Author: Won Eunji, 21500420@handong.edu
*Written: 2017/3/18
*program name: hw1_2
*
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
int pid;
pid = fork(); // create new process
wait(NULL); // parent process will wait for child process completion

/* And only the child process was executed with the if statement.
The reason is that the parent process also executed the execlp() and the message was output twice. */

if(pid == 0){
execlp("./hello", "hello", NULL);
printf("This is hw1_2.\n");
}

return 0;
}

/*
2
When execlp() is run, the child process is not the same process as the parent.
The child process runs a new process called "hello" that we entered in execlp().
That is why the print statements on the next line will not be executed.
If you want to print, you can write a print statement on the line before execlp() runs.This will print "This is hw1_2."
*/




