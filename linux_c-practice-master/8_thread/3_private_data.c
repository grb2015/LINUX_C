/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：3_private_data.c
*   breif：线程的私有数据
*   history：renbin.guo created2017-05-24
*   note：
*   usage：
*   	gcc 3_private_data.c -o 3 -lpthread
*   	[root@grb_host 8_thread]# ./3
*   	main thread begins running
*   	thread2 1617291008 is running
*   	thread1 1627780864 is running
*   	thread2 1617291008 returns this is thread2
*   	thread1 1627780864 returns this is thread1
*   	thread  ends,param = this is thread2
*   	thread  ends,param = this is thread1
*   	main thead exit
*   	[root@grb_host 8_thread]# 
*
================================================================*/

#include  <stdio.h>
#include  <pthread.h>
#include  <string.h>
#include  <stdlib.h>

pthread_key_t key;


/**
 * @brief     打印当前线程的ID,函数的参数，释放掉str指针所指向的内存空间，然后将其指向NULL
 *
 * @param str
 */
void destructor(char *str)
{
	printf("thread %s ends,param = %s\n",pthread_self(),str);
	free(str);
	str = NULL;
}

/**
 * @brief  线程1,功能就是创建一个私有数据selfstr,退出的时候答应线程ID和私有数据
 *
 * @param arg
 *
 * @return 
 */
void* thread1(void *arg)
{
	char *selfstr = (char *)malloc(20);

	memset(selfstr,0,20);
	strcpy(selfstr,"this is thread1");
	printf("thread1 %d is running\n",pthread_self());
	// key作为一个全局的键值,线程1中将它绑定为私有数据selfstr
	pthread_setspecific(key,(void*)selfstr);  /* 设置私有数据 */
	sleep(3);
						/* 获取私有数据 */
	printf("thread1 %d returns %s\n",pthread_self(),pthread_getspecific(key));
	sleep(4);
}



/**
 * @brief  线程2,功能就是创建一个私有数据selfstr,退出的时候答应线程ID和私有数据
 *
 * @param arg
 *
 * @return 
 *
 * 
 */
void* thread2(void *arg)
{
	char *selfstr = (char *)malloc(20);

	memset(selfstr,0,20);
	strcpy(selfstr,"this is thread2");
	printf("thread2 %d is running\n",pthread_self());
	// key作为一个全局的键值,线程1中将它绑定为私有数据selfstr
	pthread_setspecific(key,(void*)selfstr);
	sleep(1);  // 与线程1不同
	printf("thread2 %d returns %s\n",pthread_self(),pthread_getspecific(key));
	sleep(6); // 与线程1不同
}


/**
 * @brief  main函数中启动两个线程。他们的私有数据为Key.
 *
 * @param argc
 * @param argv[]
 *
 * @return 
 *
 * note :
 * 	按8-2.c中的说法，这里main并没有线程等待threa1和thread2结束啊，那他们如何释放资源。
 */
int main(int argc, char *argv[])
{
	pthread_t thid1,thid2;
	printf("main thread begins running\n");
	// destroy会在线程会出时,以key关联的数据为参数执行
	pthread_key_create(&key,(void*)destructor);
	pthread_create(&thid1,NULL,thread1,NULL);
	pthread_create(&thid2,NULL,thread2,NULL);
	sleep(8);
	pthread_key_delete(key);
	printf("main thead exit\n");
	return 0;
}


