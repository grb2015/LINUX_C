/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			server.c
*   breif:			通过有名管道实现非亲属进程间的通信.创建了两个有名管道，实现聊天	
*   history：		2017-06-02renbin.guo created
*   note：		
*   				bug:
*   						程序在wfd = open(FIFO_WRITE,O_WRONLY);会阻塞不动了。
*   usage：		
*
*
================================================================*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>  //exit
#include <string.h> // strerror,strlen

#define     BUF_SIZE	1024
#define     FIFO_WRITE	"writefifo"
#define     FIFO_READ	"readfifo"

int main(int argc, char *argv[])
{
		int		wfd;
		int 	rfd; 

		char 	buf[BUF_SIZE]={1,2,3};
		int 	len;

		umask(0);
		/* 创建管道 FIFO_WRITE */
		if(mkfifo(FIFO_WRITE,S_IFIFO|0666) == -1)
		{
			printf("mkfifo failed:%s\n",strerror(errno));
			exit(1);
		}

		/* 打开writefifo管道文件和readfifo管道文件*/
		umask(0);
		wfd = open(FIFO_WRITE,O_WRONLY);
		if(wfd == -1)	
		{
			printf("open FIFO error :%s",strerror(errno));
			exit(1);
		}
		else
		{
				printf("open writefifo successful\n");
		}

		while( (rfd = open(FIFO_READ,O_RDONLY)) == -1) 
		{
			sleep(1);
		}
	

		/* 无限循环，直到输入了quit.不断从键盘获得数据,然后写到writefifo,接着从readfifo读取数据 */
		while(1)
		{
				printf("server:");
				/* 从键盘获取输入 */
				fgets(buf,BUF_SIZE,stdin);
				buf[strlen(buf) -1] = '\0';		/* 注意这里strlen -1 是因为fgets最后一个字符是'\n'? */
				/* 如果输入的是 "quit",则退出*/
				if(strncmp(buf,"quit",4) == 0)
				{
					close(wfd);
					unlink(FIFO_WRITE);
					close(rfd);
					exit(0);
				}
				/* 否则,则将从键盘输入的内容写入wfd */
				write(wfd,buf,strlen(buf));
				/* 从rfd读数据 */
				len = read(rfd,buf,BUF_SIZE);
				if (len > 0)
				{
						buf[len] = '\0';
						printf("Client:%s\n",buf);
				}
		}

}

