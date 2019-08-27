/*================================================================
 *   Copyright (C) 2017 free for learn.
 *   
 *   file:           4_cleanup.c
 *   breif:          线程清理程序
 *   history：       2017-06-08renbin.guo created
 *   note：      
 *   				 push中注册的清理函数满足下列条件之一就会被调用
 *
 * 					 在push和pop之间
 * 					 1.调用了pthraed_exit
 * 					 2.响应其它线程的取消请求
 * 					 3.用非零execute参数调用了pop时
 * 			
 *   				 	
 *   				
 *   usage：     
 *   				[root@grb_host 11_pthread]# ./cleanup 
 *   				thead2: thread 2 start
 *   				thead2: thread 2 push complete
 *   				cleanup: thead2: thread 2 second handler
 *   				cleanup: thead2: thread 2 first handler
 *   				thead1: thread 1 start
 *   				thead1: thread 1 push complete
 *   				main: thread 1 exit code 1
 *   				main: thread 2 exit code 2
 *
 * 			
  ================================================================*/

#include "apue.h"
#include <pthread.h>

void
cleanup(void *arg)
{
		printf("cleanup: %s\n", (char *)arg);
}

void *
thr_fn1(void *arg)
{
		printf("thead1: thread 1 start\n");
		/* push和pop之前如果线程有退出,则会调用push中注册的函数，这里是cleanup,cleanup参数为"thread 1 first handler" */
		pthread_cleanup_push(cleanup, "thead1: thread 1 first handler");
		pthread_cleanup_push(cleanup, "thead1: thread 1 second handler");
		printf("thead1: thread 1 push complete\n");
		if (arg)
				return((void *)1);			// 注意这座椅里由于是用的return ，所以它不会被cleanup不会被调用
		pthread_cleanup_pop(0);
		pthread_cleanup_pop(0);			// 如果这里参数为非0，则也会被调用.但是是0，所以线程1的cleanup不会被调用
		return((void *)1);
}

void *
thr_fn2(void *arg)
{
		printf("thead2: thread 2 start\n");
		pthread_cleanup_push(cleanup, "thead2: thread 2 first handler");
		pthread_cleanup_push(cleanup, "thead2: thread 2 second handler");
		printf("thead2: thread 2 push complete\n");
		if (arg)
				pthread_exit((void *)2);			/* 用的是pthread_exit，所以cleanup会别调用,虽然下面参数是0 */
		pthread_cleanup_pop(0);		
		pthread_cleanup_pop(0);
		pthread_exit((void *)2);
}

int
main(void)
{
		int			err;
		pthread_t	tid1, tid2;
		void		*tret;
		/* 1.创建线程1 参数为(void*)1 */
		err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
		if (err != 0)
				err_exit(err, "can't create thread 1");

		/* 2.创建线程2 参数为(void*)1 */
		err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
		if (err != 0)
				err_exit(err, "can't create thread 2");


		/* 3.主线程等待线程1结束 ,并打印线程1的结束值*/
		err = pthread_join(tid1, &tret);
		if (err != 0)
				err_exit(err, "can't join with thread 1");
		printf("main: thread 1 exit code %ld\n", (long)tret);


		/* 4.主线程等待线程2结束 ,并打印线程2的结束值*/
		err = pthread_join(tid2, &tret);
		if (err != 0)
				err_exit(err, "can't join with thread 2");
		printf("main: thread 2 exit code %ld\n", (long)tret);
		exit(0);
}
