/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			2_exit_status.c
*   breif:			pthread_exit()的退出码可以通过pthread_join来接收
*   history：		2017-06-07renbin.guo created
*   note：		
*   				函数原型:
*   				int pthread_join(pthread_t thread, void **retval);	// 注意第二个参数
*					void pthread_exit(void *retval);		//第二个参数为retval倒是合理的.
*
*					疑惑2:	为什么pthread_join第二个参数是void **类型，而不是void*类型呢?
*   				疑惑3：	printf("thread 1 exit code %ld\n", (long)tret);		// 这里为什么又是一个(long)tret ?

*
*   usage：		
*   				[root@grb_host 11_pthread]# ./2_exit_status 
*   				thread 2 exiting
*   				thread 1 returning
*   				thread 1 exit code 1
*   				thread 2 exit code 2
*   				[root@grb_host 11_pthread]# 	
*
*   				1.return 和 pthread_exit等价
*
================================================================*/


#include "apue.h"
#include <pthread.h>


/*
 * 	注意返回值是一个指针(void *的指针)
 * 
 * 
 * */

void *
thr_fn1(void *arg)
{
		printf("thread 1 returning\n");
		return((void *)1);		// 这里返回地址0x1 ,对，它是一个指针类型,而指针类型有多少位呢?就是机器中的sizeof(int) 
								//所以这里的0x1到底有多少位，是不确定的。
							   //  一般就是long，即地址都是用long类型存的，所以地址也是一个长整形嘛
}

void *
thr_fn2(void *arg)
{
		printf("thread 2 exiting\n");
		pthread_exit((void *)2);
}

int
main(void)
{
		int			err;
		pthread_t	tid1, tid2;
		void		*tret;
		/* 1.创建线程1  */
		err = pthread_create(&tid1, NULL, thr_fn1, NULL);
		if (err != 0)
				err_exit(err, "can't create thread 1");

		/* 2.创建线程2 */
		err = pthread_create(&tid2, NULL, thr_fn2, NULL);
		if (err != 0)
				err_exit(err, "can't create thread 2");

		/* 主线程调用join等待线程1结束,获得并打印线程1的返回值 */
		err = pthread_join(tid1, &tret);		//这里tret是一个指针,把这个指针的的地址传进去
		if (err != 0)
				err_exit(err, "can't join with thread 1");
		printf("thread 1 exit code %ld\n", (long)tret);		// 这里为什么又是一个(long)tret ,tret是一个指针存放的是地址1,
															// 但是指针类型有多少位不清楚(一般来说是long相同的位数)
															// 这里，为了%ld所以转为一个长整形来打印。

		printf("#### thread 1 exit code %x\n", tret);		// 这里为什么又是一个(long)tret ,tret是一个指针存放的是地址1,
		/* 主线程调用join等待线程1结束,获得并打印线程1的返回值 */
		err = pthread_join(tid2, &tret);
		if (err != 0)
				err_exit(err, "can't join with thread 2");
		printf("thread 2 exit code %ld\n", (long)tret);

		/* 进程退出 */
		exit(0);
}

