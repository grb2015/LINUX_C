/*
 * echostat.c
 *   reports current state of echo bit in tty driver for fd 0
 *   shows how to read attributes from driver and test a bit*
 *   
 *   brief :读取标准输入的ECHO位是否开启
 *   renbin.guo created 2017-04-05
 *
 * */

#include<stdio.h>
#include<termios.h>	// 终端tty相关的

int main()
{
	struct termios info;	// termios 结构体
	int	rv;

	/* int tcgetattr(int fd, struct termios *termios_p); */

	rv = tcgetattr(0,&info);	// 0应该是代表标准输入, struct termios* termios 结构体指针作参数，传入的是地址

	if(rv == -1)
	{
		perror("tcgetattr");
		exit(1);
	}
	
	/*
	 * 	 c_flag  控制output相关属性
	 * 	 ECHO 代表是否回显 
 	 *  	
 	*/
	if(info.c_lflag & ECHO) 
		printf("echo is on !\n");
	else
		printf("echo is off !\n");
}
		

	

