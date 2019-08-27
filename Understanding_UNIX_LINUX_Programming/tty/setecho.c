/* setecho.c
 * 	usage  ./setecho [y|n]
 * 	show : how to set tty attributes
 *
 * 	演示如何设置tty 的属性
 * 	(设置标准输入的 ECHO为例)
 */

#include<stdio.h>
#include<termios.h>

/* 宏定义,替代函数*/
#define oops(s,x)  {perror(s);exit(x);}

int main(int argc,char **argv)
{
	struct termios info;
	if( tcgetattr(0,&info) == -1)
	{
		oops("tcgetattr",1);
	}

	/*
	 *	ECHO 是类似于 0001000这样的
 	 *
 	 */
	if(argv[1] == 'y')
		info.c_lflag |= ECHO ; 	 // turn on  bit
	else
		//info.c_lflag |= ECHO ; 	 // turn on  bit
		info.c_lflag  &= ~ECHO ;  // turn off 
		
	/*
 	*	现在已经在 buf struct info里面设置好了
 	*	接下来要把它写入fd = 0 即标准输入的 info里面 
 	*	int tcsetattr(int fd, int optional_actions,
 	*	                     const struct termios *termios_p);
 	* 	TCSANOW the change occurs immediately.
 	*/
	if ( tcsetattr(0,TCSANOW,&info) == -1 )
		oops("tcsetattr",2);	// 2 是什么
}
	
	
	
