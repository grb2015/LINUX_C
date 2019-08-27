/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：2_fork.c
*   breif：   to show: 父子进程拥有独立的地址空间。变量k父子进程中是独立的。
*   history：2017-05-17 renbin.guo created.
*   note：
		usage:
		[root@grb_host 7_process]# ./2

		Process Creation Study
		Parent process is running
		Child process is running
		Parent process is running
		Child process is running
		Parent process is running
		Child process is running
		Parent process is running
		Parent process is running

		[root@grb_host 7_process]#		
 *
 ================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t   pid;
	char *  msg;
	int     k;

	printf("Process Creation Study\n");

	pid = fork();

	switch(pid) {
		case 0:
			msg = "Child process is running";
			k = 3;
			break;
		case -1:
			perror("Process creation failed\n");
			break;
		default:
			msg = "Parent process is running";
			k=5;
			break;
	}
	
	while(k > 0)
	{
		puts(msg);
		sleep(1);
		k--;
	}

	exit(0);
}



