/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：1_pipe.c
*   breif：	 管道用于有亲属关系的进程间通信（父子进程).
*   history：renbin.guo created 2017-05-31
*   note：
*   usage：
*   	[root@grb_host IPC]# gcc 1_pipe.c -o 1
*   	[root@grb_host IPC]# ./1
*   	read from pipe:hello,pipe
*   	[root@grb_host IPC]# 
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * @brief  		从fd中读取数据100字节数据
 *
 * @param fd :  从fd中读的文件描述符
 */
void read_from_pipe(int fd)
{
	char message[100];
	read(fd,message,100);
	printf("read from pipe:%s",message);
}



/**
 * @brief     向文件描述符fd中写一段数据
 *
 * @param fd
 */
void write_to_pipe(int fd)
{
		char *message = "hello,pipe\n";
		write(fd,message,100);
}




/**
 * @brief 	创建一个管道，其中,父进程想管道中写数据，子进程从
 * 			管道中读取数据。从而实现进程间通信
 *
 * 
 */
int main(int argc, char *argv[])
{
		int		fd[2];
		pid_t 	pid;
		int		stat_val;

		/* 创建管道，注意参数是fd[2] */
		if (pipe(fd))
		{
				printf("create pipe failed ");
				exit(1);
		}
		pid = fork();
		switch (pid)
		{
				case -1:
						printf("fork error\n");
						break;
						
				/* 子进程关闭fd1 ,由于子进程会复制父进程的文件描述符(但是他们都指向同一个文件),所以子进程中会有fd[2],父进程中也有fd[2]*/
				/* 并且他们指向的文件是一样的，比如子进程中的fd[0]和父进程中的fd[0],他们指向的文件是一样的。 */
				case 0:
						close(fd[1]);
						read_from_pipe(fd[0]);
						exit(0);
						break;
				/* 父进程关闭fd0 */
				default:
						close(fd[0]);
						write_to_pipe(fd[1]);
						/* 父进程等待子进程结束 */
						wait(&stat_val);
						exit(0);
						break;
		}
		return 0;
}


