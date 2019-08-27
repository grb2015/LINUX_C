/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：5_condition.c
*   breif：条件变量的用法
*   history：renbin.guo created 2017-05-24
*   note：
*   	confuse:
*			pthread_cleanup_push()
*			和
*			pthread_cleanup_pop()
*			的作用是什么?
*		就是push和pop之间的代码，如果异常终止，那么他们就会调用push中的回到函数，释放锁。那为什么thread2中不设置呢?
*
*   usage：
*   	
*   	[root@grb_host 8_thread]# ./5 
*   	thread2 is running
*   	thread1 is running
*   	thread2 applied the condition
*   	thread1 applied the condition
*   	thread2 is running
*   	thread2 applied the condition
*   	thread1 is running
*   	thread1 applied the condition
*   	thread2 is running
*
* 		线程1,2能正确的互斥的到条件变量,从而执行
*
*
================================================================*/

#include  <stdio.h>
#include  <unistd.h>
#include  <pthread.h>

pthread_mutex_t mutex;	// 互斥锁
pthread_cond_t	cond;	// 条件变量

void* thread1(void*arg)
{
	/* 注册回调函数 */
	pthread_cleanup_push(pthread_mutex_unlock,&mutex);	

	while(1)
	{
			printf("thread1 is running\n");
			// 解锁
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond,&mutex);	// 等待条件变量
			printf("thread1 applied the condition\n");  // 获得条件变量
		 	pthread_mutex_unlock(&mutex); // 解锁 ,注销掉这句试试,这样的话线程1也永远得不到锁了

			sleep(1);
	}
	pthread_cleanup_pop(0);
}

void* thread2(void *arg)
{
	while(1)
	{
		printf("thread2 is running\n");
		
		pthread_mutex_lock(&mutex);   // 加锁,对pthread_cond_wait函数加锁,它只能互斥执行
		pthread_cond_wait(&cond,&mutex);  //等待条件变量
		printf("thread2 applied the condition\n"); // 获得条件变量
		pthread_mutex_unlock(&mutex);	// 解锁
		sleep(1);
	}
}
int main(int argc, char *argv[])
{
	pthread_t tid1;
	pthread_t tid2;

	/* 初始化互斥锁 */
	pthread_mutex_init(&mutex,NULL);
	/* 初始化条件变量 */
	pthread_cond_init(&cond,NULL);

	pthread_create(&tid1,NULL,(void*)thread1,NULL);
	pthread_create(&tid2,NULL,(void*)thread2,NULL);

	
	/* 线程被条件变量阻塞后，可通过pthread_cond_signal激活一个等待成立的线程，存在多个等待线程时，按入队顺序激活第一个*/

	do
	{
		pthread_cond_signal(&cond);
	}while(1);

	sleep(50);
	// pthread_eixt只会退出当前线程，而exit则会退出整个进程
	pthread_exit(0);
}




