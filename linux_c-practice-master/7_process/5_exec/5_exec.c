
/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：5_exec.c
*   breif：	  fork一个子进程,去执行新的映像.父进程等待子进程结束
*   history：2017-05-18 renbin.guo created.
*   note：
*   usage：
		[root@grb_host 5_exec]# gcc processimage.c  -o processimage

		[root@grb_host 5_exec]# ./5
			parent process  is running
			child process is running
			argv[0] = ./5
			[root@grb_host 5_exec]# ./5  parm1 parm2 parm3
			parent process  is running
			child process is running
			argv[0] = ./5
			argv[1] = parm1
			argv[2] = parm2
			argv[3] = parm3
		[root@grb_host 5_exec] 
 *
================================================================*/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char const *argv[],char **environ)
{
	int pid;
	int stat_val;
	pid = fork();
	if(pid == -1)
			printf("fork error \n");
	else if(pid > 0)
		printf("parent process  is running\n");
	else if(pid == 0)	
	{
		printf("child process is running\n");
		execve("processimage",argv,environ);

		// can never be execute
		printf("child process never go to here!\n");
	}
	wait(&stat_val);
	return 0;
}


