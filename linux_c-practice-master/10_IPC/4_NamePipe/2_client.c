/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			client.c
*   breif:		
*   history：		2017-06-02renbin.guo created
*   note：	
*   				   bug:
*   				     程序在 
						if( (wfd = open(FIFO_WRITE,O_WRONLY)) == -1)会阻塞不动了。	
*   usage：		
*
================================================================*/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>


#define		BUF_SIZE	1024
/* 注意这里READ的却是writefifo ,与server.c里面是相反的*/
#define     FIFO_READ	"writefifo"	
#define     FIFO_WRITE	"readfifo"
int main(int argc, char *argv[]) {
		int 	wfd;
		int 	rfd;

		char buf[BUF_SIZE];
		int 	len;

		umask(0);
		/* 创建管道readfifo*/
		if(mkfifo(FIFO_WRITE,S_IFIFO|0666))
		{
				printf("create writefifo failed:%s",strerror(errno));
				exit(1);
		}

		/* 打开FIFO_READ和FIFO_WRITE两个管道 */
		if( (wfd = open(FIFO_WRITE,O_WRONLY)) == -1)
		{
				printf("open FIFO_WRITE failed:%s\n",strerror(errno));
				exit(1);
		}
		while( (rfd = open(FIFO_READ,O_RDONLY)) == -1)
		{
				sleep(1);
		}

		while(1)
		{
				len = read(FIFO_READ,buf,BUF_SIZE);
				if(len > 0)
				{
						/* server写的时候，虽然说在buf的最末尾加了一个'\0' */
						/* 但是由于read读一次只读BUF_SIZE长度,有可能还没读完, */
						/* 所以这里需要加一个'\0',就算是读完了，最后一个是'\0' */
						/* 这里覆盖一下也没事 */
						buf[len]='\0';
						printf("Server:%s\n",buf);
				}

				/* 从键盘得到输入，写进wfd,如果输入时quit则退出， */
				printf("client:");
				fgets(buf,BUF_SIZE,stdin);
				buf[strlen(buf)-1] = '\0';		/* fgets最多能读入BUF_SIZE-1个字符，因为后面后面会自动加一个'\0',即buf[strlen(buf)]='\0' */
												/* buf[strlen(buf)-1]应该是'\n'这里是为了去掉'\n'*/
				if(strncmp(buf,"quit",4) == 0)
				{
					close(wfd);
					unlink(FIFO_WRITE);
					close(rfd);
					exit(0);
				}
				write( wfd,buf,strlen(buf));
		}
}


