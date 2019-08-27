/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：6_wait.c
*   breif：	fork一个子进程后子进程睡眠5秒，父进程等待子进程结束。
*   history：2017-05-18 renbin.guo created.
*   note：
*   usage：
		[root@grb_host 7_process]# ./6
			child process will sleep 5 seconds
			child process exit coede = 37
		[root@grb_host 7_process]# 
 *
 ================================================================*/
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char const *argv[])
{
	int pid;
	int stat_val;
	pid = fork();
	if(pid == -1)
		printf("fork error\n");
	else if(pid == 0)
	{
		printf("child process will sleep 5 seconds\n");
		sleep(5);
		exit(37);		
	}
	else
	{
		// 父进程阻塞，直到子进程退出
		wait(&stat_val);
		printf("child process exit coede = %d\n",WEXITSTATUS(stat_val) );
	}
	return 0;	// 由于子进程已调用了exit(37)结束了，所以这里return 0是父进程用
}

