/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			1_threadid.c
*   breif:		
*   history：		2017-06-07renbin.guo created
*   note：		
**
*			   		1.主线程和新线程共用一个进程,所以进程ID同。
*   				2.主线程和子线程竞争CPU,我这里是主线程先执行
*   				3.主线程会调用sleep(1)等待子线程结束。不然它会成为僵尸线程
*
*   				疑问：为什么return((void *)0);		
*   usage：		
*			[root@grb_host 11_pthread]# gcc 1_threadid.c -o 1_threadid -lpthread -lapue
*   		[root@grb_host 11_pthread]# ./1_threadid 
*   		main thread: pid 4221 tid 140535231964928 (0x7fd0e8946700)
*   		new thread:  pid 4221 tid 140535231956736 (0x7fd0e8944700)
*   		[root@grb_host 11_pthread]# 

================================================================*/

#include "apue.h"
#include <pthread.h>

pthread_t ntid;

		void
printids(const char *s)
{
		pid_t		pid;
		pthread_t	tid;

		pid = getpid();
		tid = pthread_self();
		printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
						(unsigned long)tid, (unsigned long)tid);
}

		void *
thr_fn(void *arg)
{
		printids("new thread: ");


		/* 这里返回的0就是代表地址0x0,即0号地址!  */
		return((void *)0);		/* 注意这里(void*)0 */
}

		int
main(void)
{
		int		err;

		err = pthread_create(&ntid, NULL, thr_fn, NULL);
		if (err != 0)
				err_exit(err, "can't create thread");
		printids("main thread:");
		sleep(1);
		exit(0);
}


