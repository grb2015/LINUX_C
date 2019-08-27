/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：1_create.c
*   breif：	  创建线程。主线程和新线程各自打印自己的ID
*   history：2017-05-19 renbin.guo created.
*   note：
*   usage：
*   	 gcc 1_create.c  -o 1 -lpthread
*   	 [root@grb_host 8_thread]# ./1
*   	 main thead id = 2078766848
*   	 [root@grb_host 8_thread]# 
*
* 	bug: 并没有打印出print_id,线程1没有被启动啊
* 	2017-10-19 added  should add sleep(1)  just like fork() in 7_process

	[root@localhost 8_thread]# ./1
	main thead id = 1639962368
	created thread  id = 1631831808
	[root@localhost 8_thread]# 


*
*   	 
*
================================================================*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<pthread.h>

void   print_id()
{
	printf("created thread  id = %d\n", pthread_self());
}
int main(int argc, char const *argv[])
{
	pthread_t thid;
	printf("main thead id = %d\n", pthread_self());
	
	//  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	//                            void *(*start_routine) (void *), void *arg);
	//                            /* 注意这里将print_id函数指针作了一次类型转换,转为了(void*) */
	if(pthread_create(&thid,NULL,(void *)print_id,NULL)  != 0)
	{
		printf("creatd failed");
		exit(1);
	}
	sleep(1);
	return 0;
}
