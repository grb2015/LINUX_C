/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：4_mutex_luck.c
*   breif：互斥锁的用法
*   history：renbin.guo created2017-05-24
*   note：
*   	线程同步有三种手段：
*   		互斥锁
*   		条件变量
*   		异步信号
*   usage：
*
================================================================*/

/* usage of mutex lock*/

#include  <pthread.h>

pthread_mutex_t	number_mutex;
int		globle;


/* 初始化锁的两种方法 */
pthread_mutex_t mutxt = PTHREAD_MUTEX_INITIALIZER
int pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutexattr_t * mutexattr);


/**
 * @brief 
 * 	功能：通过加锁实现对全局变量globle的互斥访问 
 * @note:
	pthread_mutex_lock(&number_mutex);
	加锁时，如果当前锁被占用,则阻塞等待。
 */
void write_globle()
{	
	/* 加锁,参数为number_mutex */
	pthread_mutex_lock(&number_mutex);
	globle++;
	/* 解锁 */
	pthread_mutex_unlock(&number_mutex);
}

int read_global()
{
	int temp;

	/* 加锁 */
	pthread_mutex_lock(&number_mutex);
	globle++;
	/* 解锁 */
	pthread_mutex_unlock(&number_mutex);
	return (temp);
}


/* 销毁锁 */
int pthread_mutex_destroy(pthread_mutex_t *mutex);



