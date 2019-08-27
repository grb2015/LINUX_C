/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：2_dual_pipe.c
*   breif：通过创建两个管道实现一个全双工的管道
*   history：renbin.guo created2017-05-31
*   note：
*   		1_pipe.c中的管道是半双工的，即一端只能读，另一端只能写。
*   		这里实现的全双工则是两端都可以读和写.
*   		
*   		read函数只是一个通用的读文件设备的接口。是否阻塞需要由设备的属性和设定所决定。一般来说，读字符终端、网络的socket描述字，管道文件等，*			这些文件的缺省read都是阻塞的方式。如果是读磁盘上的文件，一般不会是阻塞方式的。但使用锁和fcntl设置取消文件O_NOBLOCK状态，也会产生阻塞			*的read效果。
*
*   usage：
*   		[root@grb_host 10_IPC]# gcc 2_dual_pipe.c -o 2
*   		[root@grb_host 10_IPC]# ./2
*   		parent process read from pipe:from child process!
*   		child process read from pipe:from parent process!
*   		[root@grb_host 10_IPC]#
*
*
*   		为什么调用了wait，父进程还会先与子进程执行了呢?
*   		调用wait语句之前的内容，父子进程的执行顺序是不确定的
*   		执行到wait时，父进程才阻塞，直到子进程结束。就打印结果而言，
*   		这个实例中，执行顺序是这样的：
*
*   		父进程调用write
*   			|
*   		父进程调用read (由于没有read的内容，所以父进程会阻塞在这里.read读数据并不总是阻塞，取决于读什么设备，以及open时候的设置)
*   			|
*   		子进程调用wrte
*   			|
*   		父进程阻塞被结束，调用read得到数据
*   			|
*   		父进程调用wait等待子进程结束
*   			|
*   		子进程调用read得到数据
*   			
*
*
*   		
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



/**
 * @brief 	先向writefd文件描述符写一段数据，然后从readfd文件描述符读100字节数据
 *
 * @param 	readfd
 * @param 	writefd
 */
void child_rw_pipe(int readfd,int writefd)
{
	char *message1 = "from child process!\n";
	write(writefd,message1,strlen(message1)+1);

	char message2[100];
	read(readfd,message2,100);
	printf("child process read from pipe:%s",message2);
}


/**
 * @brief 	先向writefd文件描述符写一段数据，然后从readfd文件描述符读100字节数据
 *
 * @param 	readfd
 * @param 	writefd
 */
void parent_rw_pipe(int readfd,int writefd)
{
	char *message1 = "from parent process!\n";
	write(writefd,message1,strlen(message1)+1);

	char message2[100];
	read(readfd,message2,100);
	printf("parent process read from pipe:%s",message2);
}

int main(int argc, char *argv[])
{
		int		pipe1[2],pipe2[2];
		pid_t	pid;
		int 	stat_val;

		if (pipe(pipe1))
		{
			printf("pipe1 fialed!");
			exit(1);
		}	
		if (pipe(pipe2))
		{
			printf("pipe2 fialed!");
			exit(1);
		}

		pid = fork();
		switch (pid)
		{
				case -1:
						printf("fork error");
						exit(1);
						break;
				/* 子进程只使用pipe1的写端，pipe2的读端 */
				case 0:
						close(pipe1[1]);  //关闭pipe1的读端
						close(pipe2[0]);  // 关闭pipe2的写端
						child_rw_pipe(pipe1[0],pipe2[1]);
						exit(0);
						break;
				/* 父进程与子进程相反 */
				/* 父进程只使用pipe1的读端，pipe2的写端 */
				default:
						close(pipe1[0]);  
						close(pipe2[1]);  
						parent_rw_pipe(pipe2[0],pipe1[1]);
						wait(&stat_val);
						exit(0);
						break;
		}

}


