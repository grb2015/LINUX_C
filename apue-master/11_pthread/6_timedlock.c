/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			6_timedlock.c
*   breif:			超时版本的lock  pthread_mutex_timedlock()
*   history：		2017-06-10renbin.guo created
*   note：		
*   usage：		
*   		 		gcc 6_timedlock.c -o 6 -lrt(注意要连接上rt库)
*
*   		 		[root@grb_host 11_pthread]# ./6 
*   		 		mutex is locked
*   		 		current time is 06:53:55 PM
*   		 		the time is now 06:54:05 PM
*   		 		can't lock mutex again: Connection timed out
*   		 		[root@grb_host 11_pthread]# 
*
*
================================================================*/


#include "apue.h"
#include <time.h>
#include <pthread.h>

int
main(void)
{
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");

	/* 获得当前时间 */
		/* 1.得到tout */
	clock_gettime(CLOCK_REALTIME, &tout);
		/* 2.通过tout得到tmp*/
	tmp = localtime(&tout.tv_sec);
		/* 3.通过tmp得到buf,buf存放当前时间*/
	strftime(buf, sizeof(buf), "%r", tmp);
		/* 4.打印buf */
	printf("current time is %s\n", buf);


	tout.tv_sec += 10;	/* 10 seconds from now */
	/* caution: this could lead to deadlock */
	err = pthread_mutex_timedlock(&lock, &tout);

	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("the time is now %s\n", buf);

	/* 对本例而言,timeout函数是无法得到锁的，所以else */
	if (err == 0)
		printf("mutex locked again!\n");
	else
		printf("can't lock mutex again: %s\n", strerror(err));
	exit(0);
}
