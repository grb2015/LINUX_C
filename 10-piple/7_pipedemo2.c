/*
 * brief : demostates how pipe is duplicated in fork()
 * 	   parent continues to write and read pipe.
 * 	   but child also write to the pipe
 * history: 2017-04-29 renbin.guo created
 * 	    2017-05-01 renbin.guo modifed
 *
 * // 2017-05-01 renbin.guo addded
 * note:   当进程创建一个管道之后,fork,则子进程也会得这个管道，父子进程 *	  都可以从管道中读写。一般而言，是一个进程读，一个进  *        *	 程写，从而实现进程间通信
 *
 *         疑问1. read这里会阻塞吗，如果读缓冲区内没有数据?
 */

#include<stdio.h>

#define CHILD_MESS "I want a cookie\n" // child  message
#define PAR_MESS   "testing...\n"      // parent message
#define oops(m,x)  {perror(m);exit(x);}

int main()
{
	int pipefd[2];
	int len;
	char buf[BUFSIZ];
	int read_len;
	
	
	if(pipe(pipefd) == -1)
		oops("cannot fork",2);
	switch(fork())
	{
	case -1:
		oops("connot fork",2);
	case 0:
		len = strlen(CHILD_MESS);
		// 子进程向管道中写
		// 一直循环，直到写失败
		while(1)
		{
			if(write(pipefd[1],CHILD_MESS,len) != len)
				oops("write",1);
			sleep(5);
		}
	default:
		len = strlen(PAR_MESS);
		// 一直循环,直到read_len <= 0
		// 父进程从管道中读 
		while(1)
		{
			// 父进程也是可以向管道中写的
			/*if(write(pipefd[1],PAR_MESS,len) != len)
				oops("write",4);
			sleep(1);*/

			// 有个疑问，read这里会阻塞吗，如果读缓冲区内没有数据?,如果不阻塞，这里read_len就会小于0了
			read_len = read(pipefd[0],buf,BUFSIZ);
			if(read_len <= 0)
				break;

			/* 输出到屏幕 打印*/
			write(1,buf,read_len);
		}
	}
}
			
