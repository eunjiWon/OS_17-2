/*
*Author: Won Eunji, 21500420@handong.edu
*Written: 2017/3/18
*program name: hw1_1
*
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
fork();
wait(NULL);
fork();
wait(NULL);
fork();
wait(NULL);
printf("Hello, World!\n");
return 0;
}

/*
1_1
(The number of fork = f and the number od messages output = m.)
The relation between the number of messages output and the number of fork() is 2^f = m.
The reason is as follows. 
When f = 1, m = 2 because both the parent and child print the message.
When f = 2, the total number of messages is four because both the parent and child have child processes and two child processes print messages.
when f = 3, all four processes have children, and all four child processes print messages, so the total number of messages is eight.
*/
/*
1_2
Called seven times.
When P1 calls fork(), it creates a new process called P2.
And P1 and P2 execute fork() on the next line.
Then P1 creates P3.
And P2, a child of P1, also makes a new child P4.
P1, P2, P3, and P4 execute fork() on the next line.
P1 makes P5, P2 makes P6, P3 makes P7, and P4 makes P8.
In conclusion, the process that called fork is as follows.

p1(When making P2)
----------------------
p1(When making P3)

p2(When making P4)
----------------------
p1(When making P5)

p2(When making P6)

p3(When making P7)

p4(When making P8)

A total of 7 calls are executed.
*/
/*
1_3
0 appears seven times.
Pid is the value of constant that will be received as the return value when calling fork.
At this time, the parent process receives a value greater than 0, and the child process receives a zero(becaus the child process has not yet called fork).
Therefore, pid values of P2, P3, P4, P5, P6, P7, and P8 are 0.
*/


























