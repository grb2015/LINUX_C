#include<stdio.h>
#include<signal.h>

void wakeup(int);

int main()
{
	printf("about to sleep for 4 seconds\n");

	/* 除非设置了SIGALRM信号处理函数，不然信号将杀死当前进程 */
	signal(SIGALRM,wakeup);	/*注册ALRM信号处理函数*/

	alarm(4);	// 4 s后发SIGALRM信号

	/* pause: causes the calling process (or thread) to sleep until a signal is delivered */
	pause();	// 任何信号都可唤醒进行，而不只是SIGALRM

	printf("Morning so soon?\n");
}
void wakeup(int signum)
{
	printf("Alarm received from kernal\n");
	printf("signum = %d\n",signum);
}
