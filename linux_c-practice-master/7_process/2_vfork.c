/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：2_vfork.c
*   breif：	
		fork完全复制父进程的地址空间,开销较大。而vfork则是与父进程共享地址空间，不复制。

		1.vfork的子进程一定会先与父进程执行，直到子进程调用exec或exit后，父进程才能执行。
		2.vfork一般用于fork一个子进程，然后马上去执行exec,这样就没有必要完全复制父进程地址空间，减少开销

		下面的例子通过父子进程的执行顺序和对变量的修改来说明vfork
		
			
*   history：2017-05-18 renbin.guo created.
*   note：
*   usage：

		1. use vfork
			[root@grb_host 7_process]# ./2_fork 
				fork is diffirent with vfrok 
				Child process is running
				Child process is running
				Child process is running
				Child's globVar = 8,var = 4
				Parent process is running
				Parent process is running
				Parent process is running
				Parent process is running
				Parent process is running
				Parent's globVar = 13 ,var = 5
			[root@grb_host 7_process]#

		2. use fork
			[root@grb_host 7_process]# ./2_fork 
				fork is diffirent with vfrok 
				Parent process is running
				Child process is running
				Parent process is running
				Child process is running
				Parent process is running
				Child process is running
				Parent process is running
				Child's globVar = 8,var = 4
				Parent process is running
				Parent's globVar = 10 ,var = 6
			[root@grb_host 7_process]

 *
 ================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int globVar = 5;

int main(void)
{
	pid_t  pid;
	int    var = 1, i;
	
	printf("fork is diffirent with vfrok \n");

	pid = fork();		// replace as fork() and compare
//	pid = vfork();
	switch(pid) {
		case 0:
			i = 3;
			while(i-- > 0)
			{
				printf("Child process is running\n");
				globVar++;
				var++;
				sleep(1);
			}
			printf("Child's globVar = %d,var = %d\n",globVar,var);
			break;
		case -1:
			perror("Process creation failed\n");
			exit(0);
		default:
			i = 5;
			while(i-- > 0)
			{
				printf("Parent process is running\n");
				globVar++;
				var++;		
				sleep(1);
			}
			printf("Parent's globVar = %d ,var = %d\n", globVar ,var);
			exit(0);
	}
}



