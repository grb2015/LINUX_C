/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：2_jointhread.c
*   breif：一个可join的线程所占用的内存仅有当有线程对其执行了pthread_join后才释放
*   	   下面的程序说明，主线程等待辅助线程结束
*   history：renbin.guo created2017-05-23
*   note：
*   usage：
*   	[root@grb_host 8_thread]# gcc 2_jointhread.c  -o 2 -lpthread
*   	[root@grb_host 8_thread]# ./2
*   	I am helping to do some job
*   	assistthid's exit is caused 0
*   	[root@grb_host 8_thread]# 
*
*
================================================================*/

#include  <stdio.h>
#include  <pthread.h>
void assisthread(void *arg)
{
	printf("I am helping to do some job\n");
	sleep(3);
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
	pthread_t assistthid;
	int status;

	pthread_create(	&assistthid,NULL,(void*)assisthread,NULL);
	pthread_join(assistthid,(void*)&status);
	printf("assistthid's exit is caused %d\n",status);
	return 0;
}
