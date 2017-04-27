#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<signal.h>
#include<sys/wait.h>
/*
 *	breif  : 开辟一个子进程来执行 **argv中包含的程序	 	
 *	param  : [in] char** argv 命令参数，argv[0]为程序映像名
 *
 *	returns: 
 *		失败0: 	argv参数为空，什么都不执行
 *		成功int child_info, 子进程的返回码
 * 	
 */

int execute(char *argv[])
{
	int pid;
	int child_info = -1;
	if(argv[0] == NULL)
		return 0;
	if( (pid = fork()) == -1)
		perror("fork");
	else if(pid == 0)
	{
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
		execvp(argv[0],argv);
		// 如果正常，则永远不会执行下面这个
		perror("cannot execute commnad");
		exit(1);
	}
	else
	{
		if(wait(&child_info) == -1)
			perror("wait");
	}
	return child_info;
}
	
