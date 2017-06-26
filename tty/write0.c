/* write0.c
 *  
 *      purpose: send messages to another terminal(从当前终端的stdin里面获取输入，发送给另外的终端)
 *      method:  open the other terminal for output then
 *     	 	 copy from stdin to that terminal
 *      shows:  a terminal is just a file supporting regular i/o
 *      usage: write0 ttyname                                                                     
 *	使用方法： ./write0  /dev/pts/0 (需要直接给出终端名)
 *
 *
 * 	tty跟普通文件一样，read, write 一个tty就相当于与另外的
 * 	用户进行网络聊天了。(要是不是在一台主机上呢/?)
 *	
 *	how to run this program?
 *
        [root@localhost tty]# who
        root     pts/0        2017-06-26 17:06 (192.168.117.1)
        root     pts/1        2017-06-26 13:41 (192.168.117.1)
        root     tty1         2017-06-26 13:36 (:0)
        [root@localhost tty]# 

        开了2个终端
        
        pts0给pts1发消息
        [root@localhost tty]# ./0 /dev/pts/1
        abc
        def

        pts1接收到消息
        [root@localhost tty]# 
        abc
        def


 *
 *
 * */

#include<stdio.h>
#include<fcntl.h>	// for open
//#include< unistd.h>	// read & write
int main(int argc,char **argv)
{
	int fd ;
	char buf[BUFSIZ];
	
	if(argc != 2)
	{
		fprintf(stderr,"usage: write0 ttyname\n");
		exit(1);
	}
	fd = open(argv[1],O_WRONLY);  // 只写打开
	{	
		if(fd == -1)
		{
			perror(argv[1]);
			exit(1);
		}
	}

	/*从stdin获取输入，如果输入不为空，就一直向fd写*/
	/* fgets和gets类似，gets是从标准输入读入，而fgets可以指定文件 */	
	while(fgets(buf,BUFSIZ,stdin) != NULL)
	{
		if(write(fd,buf,strlen(buf)) == -1) 
			break;
	}
	close(fd);
}
	
	


