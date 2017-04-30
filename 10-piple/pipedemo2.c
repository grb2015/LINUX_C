/*
 * brief : demostates how pipe is duplicated in fork()
 * 	   parent continues to write and read pipe.
 * 	   but child also write to the pipe
 * history: 2017-04-29 renbin.guo created
 *
 */

#include<stdio.h>

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
		while(1)
		{
			if(write(pipefd[1],CHILD_MESS,len) != len)
				oops("write",1);
			sleep(5);
		}
	default:
		len = strlen(PAR_MESS);
		// 一直循环,直到read_len <= 0
		while(1)
		{
			if(write(pipefd[1],PAR_MESS,len) != len)
				oops("write",4);
			sleep(1);
			read_len = read(pipefd[0],buf,BUFSIZ);
			if(read_len <= 0)
				break;
			write(1,buf,read_len);
		}
	}
}
			
