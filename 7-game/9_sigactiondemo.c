/*********************************************
 * filename:	sigactdemo.c
 * author  :	renbin.guo	
 * brief   :	show use of sigaction()
 * 
 * history ：	2017-04-16 renbin.guo created
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
	/* 设置resethand restart标志 */
	newhandler.sa_flags = 	SA_RESETHAND | SA_RESTART ;

	sigemptyset(&blocked);		// blocked 承接 
	sigaddset(&blocked,SIGQUIT);   // 用得到的block 做参数，调用sigaddset
	/* 用blocked为结构体sigaction的sa_mask赋值 */
	newhandler.sa_mask = blocked;	

	/* sigaction 不是一个结构体吗，怎么又是一个函数了
	* 用上面填充的sigaction结构体对象newhandler 和 SIGINT做参数
	* 调用sigaction函数
	*/
	if(sigaction(SIGINT,&newhandler,NULL) == -1)
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
	sleep(s);
}
		

	
	


	
