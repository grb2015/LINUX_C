/*********************************************
 * filename:	sigactdemo.c
 * author  :	renbin.guo	
 * 		利用posix标准的多个信号的处理策略进行编程
 * 		show use of sigaction()
 * 		处理SIGINT时阻塞SIGQUIT
 * 		
 * 
 * history ：	2017-04-16 renbin.guo created
 *
 * note：	经典的信号处理函数是.signal,而posix中使用的是sigaction
 * 		
 * 		具体功能：
 * 			1.顺序按下^c和^\将被阻塞直到^c处理完毕
 * 			2.如果连续按下^c两次，则处理完第一个^c后,进程会被第二个^c杀死,因为是捕鼠器模型，一个信号注册函数只能作用一个信号。
 * 			3.如果要捕获所有的^c,需要将SA_RESETHAND掩码从sa_flag中去掉
 * 		
 *
 * 
 **********************************************/ 	

#include<stdio.h>
#include<signal.h>

#define		INPUTLEN	100

void	inthandler();
int main()
{
	struct sigaction  newhandler;

	/* /usr/include/asm/signal.h
 	 * typedef unsigned long sigset_t; 
 	 */
	sigset_t 	blocked ;	// unsigned long blocked
	char		x[INPUTLEN];

	/* 将结构体sigaciton的sa_handler字段赋值为自定义函数inthandler*/
	newhandler.sa_handler = inthandler;
	/* 
 	* 设置resethand restart标志 
 	*  SA_RESETHAND:当处理函数被调用时重置，也就是捕鼠器模式 
 	*  SA_RESTART  :当系统调用针对一些慢速的设备，重新开始，而不是返回。
 	*/
	newhandler.sa_flags = 	SA_RESETHAND | SA_RESTART ;


	/*  clear all bits   blocked 承接  */
	sigemptyset(&blocked);		

	/*  add SIGQUIT to list ,如果有信号在处理而SIGQUIT到来，则阻塞SIGQUIT*/
	sigaddset(&blocked,SIGQUIT);   // 用得到的block 做参数，调用sigaddset

	/* 用blocked为结构体sigaction的sa_mask赋值 
 	 *
 	 * sa_mask决定在处理一个消息时是否要阻塞其他信号
 	 */
	newhandler.sa_mask = blocked;	

	/* sigaction 不是一个结构体吗，怎么又是一个函数了
	* 用上面填充的sigaction结构体对象newhandler 和 SIGINT做参数
	* 调用sigaction函数
	*/
	if(sigaction(SIGINT,&newhandler,NULL) == -1)		// 等价于 signal(SIGINT,newhandler)
		perror("sigaction");

	else	// sigaction执行正确
		while(1)
		{
			fgets(x,INPUTLEN,stdin);
			printf("input: %s",x);
		}
}

void inthandler(int s)
{
	printf("handle signal %d begin\n",s);
	sleep(s);
	printf("handle signal %d end\n",s);
}
		

	
	


	
