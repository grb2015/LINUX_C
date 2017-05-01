/******************************************************************
 * brief :show how to redirect output for another program
 * 	  idea: fork ,then in the child ,redirect output ,then exec
 *
 *        功能: who>userlist -- 把标准输出重定向到文件userlist上
 *
 * histroy: 2017-04-30 renbin.guo creatd
 *
 ******************************************************************/

#include<stdio.h>

int main()
{
	int pid;
	int fd;
	
	printf("About to run who into  a file\n");

	if( (pid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	if(pid == 0)
	{
		close(1);  // 关闭标准输出
		// 将fd 1 指向了 userlist
		fd = creat("userlist",0644);
		execlp("who","who",NULL);
		perror("execlp");
		exit(1);
	}
	if( pid != 0) // 父进程等待子进程结束
	{
		wait(NULL);
		printf("Done running who.results in userlist\n");
	}
}
	
		
