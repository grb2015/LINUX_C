/*******************************************************************
 * 	filename:	7_ticker_demo.c
 *	author	:	renbin.guo	
 *
 *	brief	:	use interval timer to generat(产生)regular signals
 *			which are in turn(轮流,循环) caught and used to count down(倒计时)
 *
 *	history	:	2017-04-15 renbin.guo created
 *		
 *	note	:
 *
 ********************************************************************/

#include <stdio.h>
#include <signal.h>
#include<sys/time.h>

int set_ticker(int n_msecs);
void countdown(int signum);

/*
 *	功能：	计时10个500us后程序退出,这个程序比之前的7_sleep1.c更精确，它用到了interval定时器
 *
 *	具体流程：	countdown函数收到10个alrm信号后就退出整个程序。
 *			set_ticker用于定时，会每个500us产生一个arlm信号
 *
 *			注意：set_ticket 设置成功之后，系统就进入pause进行睡眠，然后等待信号来唤醒
 *
 */

int main()
{
	int count=0;
	signal(SIGALRM,countdown);

	/*
 	 * set_ticker是系统函数
 	 */
	if( set_ticker(500) == -1)
		perror("set_ticker");
	else
		while(1)
		{
			pause();
			printf("\nreceive siganl %d times ",++count);
		}
	return 0;
}
/************************************************
 *	brief	:	捕捉定时器时间到的信号，定时10次后退出程序
 *	param	:	[in] signum ：信号类型
 *	returns	:	void
 *
***********************************************/

void countdown(int signum)
{
	static int num = 10;
	num--;
	fflush(stdout);
	if(num < 0)
	{
		printf("DONE !");
		exit(0);
	}
}
/*************************************************************************************
 * 	brief  :	用传入的参数n_msecs来设置系统结构体itimerval
 * 			功能：	从现在开始过n_msecs us开始启动定时器，然后每n_msecs us发一次信号
 *
 * 	param  :	[in]n_msecs  n_msecs 毫秒后开始启动定时器,然后每n_msecs us发一次信号
 * 	returns:	
 * 		-1 : error
 * 		 0 : ok
 ****************************************************************************************/
int set_ticker(int n_msecs)
{
	struct itimerval  new_timeset;
	long   n_sec;
	long   n_usecs;
	
	n_sec = n_msecs / 1000;
	n_usecs= (n_msecs % 1000) * 1000L;	// 小数的秒数转为微秒	注意这里的'L'

	printf("n_usecs = %ld ",n_usecs);

	/*  tv_sec  : 秒数
 	 *  tv_usec ：微秒数
	 *  设置500us后启动定时器，tv_sec =0,tv_userc=500us
	 *  然后每隔500us发一次信号
	*/

	new_timeset.it_interval.tv_sec = n_sec;		//重复间隔,每隔n_sec通报一次，比如每隔4小时吃药
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;		//初始时间  类似于普通计时器，比如设为1h,代表你一小时后吃药
	new_timeset.it_value.tv_usec = n_usecs;

	/*
 	 *	进程有三个计时器。
 	 *		1.ITIMER_VIRTUAL只计算进程在用户态花费的时间
 	 *		2.ITIMER_READ	不管什么态，真真实实使用的时间
 	 *		3.
 	 */
	return setitimer(ITIMER_REAL,&new_timeset,NULL);
}
	
	


