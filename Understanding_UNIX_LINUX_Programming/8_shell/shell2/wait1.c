// 父进程等待子进程完成
//	等待过程中，按下kill childpid杀死子进程试试，看看返回的状态码
//	杀死父进程呢?
//	bug kill 并没有出现书中的情况啊?
//
#include<stdio.h>

void parent_code(int childpid)	;
void child_code(int delay);

#define DELAY 7

int main()
{
	int newpid;
	printf("I am parent process ,my pid is %d\n",getpid());
	 newpid = fork();
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
	int child_state;

	printf("I am parent process ,my pid is %d,I am wait child\n",getpid());
	// 返回的是结束的进程的pid
	// child_status取得子进程的返回状态
	wait_rv = wait(&child_state);
	printf("wait returnd  %d\n",wait_rv);
	{
	int high_8 = child_state >> 8;    //1111 1111 0000 0000
	int low_7 = child_state & 0x7F;   // 0000 0000 0111 1111
	int bit_7 = child_state & 0x80;  // 0000 0000 1000 0000
	printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);
	}
}
