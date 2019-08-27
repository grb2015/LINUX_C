/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：1_fork.c
*   author：renbin.guo 
*   history：2017-05-17 renbin.guo created.
*   note：
			shwo the use of fork()
			fork() 子进程返回0,父进程返回子进程的进程ID,出错返回-1
			
			usage:
			1.
				[root@grb_host 7_process]# ./1
				I am child process,mypid = 3358,myppid =3357 
				exit
				I am parent process,mychild pid = 3358,mypid = 3357,myppid =3314
				exit
				[root@grb_host 7_process]# 

			2. bug :如果注释掉sleep(1),为什么子进程无法退出?,直到我输入一个按键才退出。(在我的计算机上调度是父进程先退出)
	
			[root@grb_host 7_process]# ./1
			I am parent process,mychild pid = 3605,mypid = 3604,myppid =3314
			exit
			[root@grb_host 7_process]# I am child process,mypid = 3605,myppid =1 
			exit
			a
			-bash: a: command not found
			[root@grb_host 7_process]# 



						
			
			
*
================================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
 
 int main(int argc, char const *argv[])
{
	int pid;
	pid = fork();
	if(pid == 0)  // child process
		printf("I am child process,mypid = %d,myppid =%d \n",getpid(),getppid());
	else if(pid >0)
	{
		//sleep(1);	// 如果不加这个，在我们的电脑上父进程会先于子进程结束，然后子进程不会结束
		printf("I am parent process,mychild pid = %d,mypid = %d,myppid =%d\n",pid,getpid(),getppid());
	}
	else
		printf("fork error\n");
	printf("exit\n");
	exit(0);	// 这个地方是显式调用exit()退出进程，当然,没有这个也是可以的，进程会在main函数返回后被销毁
}
