/************************************************************
 *
 * brief :prompting shell version2
 * 	  solves  the one-shot problem of version1(一次只能运行一个命令)
 * 	  use ececvp(),but fork()s first so that the shell waits around 	
 *	  to perform another command 
 *
 *	  shell版本2，解决了解决了一次执行执行一条命令就退出的问题.
 *
 *	bug:shell catches siganal . press ^C (按^c终止程序，但是shell也被终止了(并没有这个现象))
 *
 *
 *下一个版本的改进：
	1. 解决按下ctrl+c会终止shell的问题
	2. 目前程序只能通过按下^c来终止，希望按下ctrl-D 或输入exit来退出
	3. 让用户能够在一行输入所有参数
 * histroy:
 * 	2017-04-22 renbin.guo created
 *
 * note:
 * 	usage:
 * 	[root@localhost shell2]# clear
 * 	[root@localhost shell2]# ./p2
 * 	arg[0]?ls
 * 	arg[1]?.
 * 	arg[2]?
 * 	fork1.c  p2  psh2.c  wait1.c
 * 	child exited with status 0,0
 * 	arg[0]?ls
 * 	arg[1]?../
 * 	arg[2]?
 * 	shell1	shell2
 * 	child exited with status 0,0
 * 	arg[0]?^C
 * 	[root@localhost shell2]# 
 *
 * 		
 **********************************************************************/
#include<stdio.h>
#include<signal.h>

#define MAXARGS 20	// 最多的参数个数
#define ARGLEN 100	// 每个参数的最大长度

void execute(char **arglist);
char *makestring(char *buf);

int main()
{
	char 	*arglist[MAXARGS+1];
	int 	numargs;
	char 	argbuf[ARGLEN];
	char 	*makestring();
	
	numargs = 0;
	while(numargs < MAXARGS)
	{
		printf("arg[%d]?",numargs);
		// 获取一个参数，存放在argbuf里.如果第一个字符是直接按下的回车，说明结束输入了
		if(fgets(argbuf,ARGLEN,stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else
		{
			if( numargs > 0)
			{
				arglist[numargs]=NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}
	return 0;
}
		
void execute(char **arglist)
{
	int pid;
	int exitstatus;
	// fork() 子进程返回0，父进程返回子进程的id 
	pid = fork();
	switch(pid)
	{
	case -1:
		perror("fork failed");
		exit(1);	
	// 如果是子进程，则执行指令
	case 0:
		execvp(arglist[0],arglist);
		perror("execvp failed");
		exit(1);
	// 如果是父进程，则等待子进程结束
	default:
		// wait , 返回的是等待结束的进程的id,如果子进程没结束，就一直等待
		while( wait(&exitstatus)!= pid )
			;
		// 子进程结束,这是打印什么?
		printf("child exited with status %d,%d\n",exitstatus>>8,exitstatus&0337);
	}

}

char *makestring(char *buf)	
{
	char *cp;
	buf[strlen(buf)-1] = '\0';
	cp = malloc(strlen(buf)+1);
	if(cp == NULL)
	{
		fprintf(stderr,"no memory\n");
		exit(1);
	}
	strcpy(cp,buf);
	return cp;
}
		
		

		
	

