/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：	3_monitor.c
*   breif： 父进程向子进程通过管道传递参数，子进程执行父进程的命令。	
*   		这里子进程为被监控进程
*   history：renbin.guo created2017-05-31
*   note：
*   usage：
*   	[root@grb_host pipe]# gcc 3_monitor.c  -o 3	
*		[root@grb_host pipe]# gcc ctrlprocess.c  -o ctrlprocess
*
*		[root@grb_host pipe]# ./3 getpid
*		in ctrlprocess : argv[0] = ./3
*		in ctrlprocess : argv[1] = getpid
*		in ctrlprocess : ctrlprocess receive :getpid
*		in ctrlprocess : my pid :2875
*
*		[root@grb_host pipe]# ./3 exit
*		in ctrlprocess : argv[0] = ./3
*		in ctrlprocess : argv[1] = exit
*		in ctrlprocess : ctrlprocess receive :exit
*		in ctrlprocess: going to exit[root@grb_host pipe]# 
*
================================================================*/

#include <stdio.h>
#include<sys/types.h>		// pid_t
#include<stdlib.h>			// exit
#include<string.h>			// strlen


int main(int argc, char *argv[],char **environ)
{
		int		fd[2];
		pid_t	pid;
	    int 	stat_val;

		if(argc < 2)	
		{
			printf("wrong parameters");
			exit(0);
		}
		if(pipe(fd))
		{
			printf("create pipe faild");
			exit(1);
		}

		pid = fork();
		switch (pid)
		{
				case -1:
			            printf("fork faild");
			            exit(1);
						break;
				/* 子进程 */
				case 0:
						/* 关闭标准输入,然后把标准输入指向管道的fd[0] */
						close(0);
						dup(fd[0]);
						/* 执行新程序，向新程序传递当前的argv和envir */
						execve("ctrlprocess",(void*)argv,environ);
						exit(0);
						break;
				/* 父进程 */
				default:
						close(fd[0]);
						/* 将命令行第一个参数写进管道 ,也即是写进标准输入，然后子进程的ctrlprocess就会从标准输入获得*/
						write(fd[1],argv[1],strlen(argv[1]));
						break;
		}
		/* 子进程在swith里面有exit,所以只有父进程可以执行到这里 */
		wait(&stat_val);	
		exit(0);
}

