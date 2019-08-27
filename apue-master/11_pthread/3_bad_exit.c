/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			3_bad_exit.c
*   breif:			pthread_join来获取线程的退出值，如果这个退出值分配在线程栈上,则线程退出后这个值不能用了 ，不然会出错。
*   history：		2017-06-07renbin.guo created
*   note：		
*   usage：		
*   				[root@grb_host 11_pthread]# ./3_bad_exit 
*   				thread 1:
*   				structure at 0x7f3e0a0dfea0
*   				foo.b = 2
*   				foo.c = 3
*   				foo.d = 4
*   				parent starting second thread
*   				thread 2: ID is 139904433391360
*   				parent:
*   				structure at 0x7f3e0a0dfea0
*   				foo.a = 0			// 可见，这里的值跟之前不一样了。
*   				foo.b = 0
*   				foo.c = 1
*   				foo.d = 0
*   				[root@grb_host 11_pthread]#		
*
================================================================*/

#include "apue.h"
#include <pthread.h>

struct foo {
		int a, b, c, d;
};

		void
printfoo(const char *s, const struct foo *fp)
{
		printf("%s", s);
		printf("  structure at 0x%lx\n", (unsigned long)fp);
		printf("  foo.a = %d\n", fp->a);
		printf("  foo.b = %d\n", fp->b);
		printf("  foo.c = %d\n", fp->c);
		printf("  foo.d = %d\n", fp->d);
}

		void *
thr_fn1(void *arg)
{
		struct foo	foo = {1, 2, 3, 4};

		printfoo("thread 1:\n", &foo);
		pthread_exit((void *)&foo);		//其实就是返回结构体foo的地址,我们不指定它按什么类型解析，char或者int,所以就是(void*)
}

		void *
thr_fn2(void *arg)
{
		printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
		pthread_exit((void *)0);
}

int
main(void)
{
		int			err;
		pthread_t	tid1, tid2;
		struct foo	*fp;

		/* 1. 创建线程1 */
		err = pthread_create(&tid1, NULL, thr_fn1, NULL);
		if (err != 0)
				err_exit(err, "can't create thread 1");


		/* 2.等待线程1结束 */
		err = pthread_join(tid1, (void *)&fp);	/* 注意这里使用了一个foo结构体的指针来存放线程1的退出值 */
		if (err != 0)
				err_exit(err, "can't join with thread 1");
		

		/* 3. 创建线程2 */
		sleep(1);
		printf("parent starting second thread\n");

		err = pthread_create(&tid2, NULL, thr_fn2, NULL);
		if (err != 0)
				err_exit(err, "can't create thread 2");

		/* 4.主线程睡眠1s,等待线程2结束 */
		sleep(1);		



		/* 5.fp之前是用来承接线程1的退出值,我们在这里使用它. */
		printfoo("parent:\n", fp);	/* 但是由于线程1已经结束了,fp指向的对象是在线程函数的栈里分配的，已经回收了，所以这里不正确*/

		exit(0);
}


