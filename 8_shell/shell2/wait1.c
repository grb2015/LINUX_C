// 父进程等待子进程完成
//
#include<stdio.h>

void parent_code(int childpid)	;
void child_code(int delay);

#define DELAY 2

int main()
{
	int newpid = fork();
	if(newpid == -1)
		perror("fork error");
	else if(newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);
}
void child_code(int delay)
{
	printf("I am child process ,my pid is %d\n",getpid());
	sleep(delay);
	exit(17);
}
void parent_code(int childpid)	
{
	//return value
	int wait_rv;
	wait_rv = wait(NULL);
	// 返回的是结束的进程的pid
	printf("wait returnd  %d\n",wait_rv);
}
