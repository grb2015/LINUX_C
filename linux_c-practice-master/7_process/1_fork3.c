/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：1_fork3.c
*   breif：  To show 如果父进程先于子进程结束，子进程就成为孤儿进程
*   history：2017-05-18 renbin.guo created.
	note:
	usage:
		[root@grb_host 7_process]# ./13
		I am parent process,my pid is 2934
		[root@grb_host 7_process]# A background process, PID:2935, ParentID: 1
		A background process, PID:2935, ParentID: 1
		A background process, PID:2935, ParentID: 1
		A background process, PID:2935, ParentID: 1
		A background process, PID:2935, ParentID: 1

 *
 ================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t  pid;

	pid = fork();
	switch(pid) {
		case 0:
			while(1)
			{
				printf("A background process, PID:%d, ParentID: %d\n",getpid(),getppid());
				sleep(3);
			}
		case -1:
			perror("Process creation failed\n");
			exit(-1);
		default:
			printf("I am parent process,my pid is %d\n",getpid());
			exit(0);
	}

	return 0;
}



