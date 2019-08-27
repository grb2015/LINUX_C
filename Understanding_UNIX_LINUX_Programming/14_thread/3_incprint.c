/* incprint.c - one thread increments, the other prints */
/*****************************************************
*  brief : 线程间通信--共享全局变量
*  history: 2017-05-09 renbin.guo created
*  note :
* 	usage:
*
*       gcc 3_incprint.c -o 3_incprint -lpthread
*
*		[root@localhost 14_thread]# ./3_incprint
*		count = 1
*		count = 2
*		count = 3
*		count = 4
*		count = 5
*		[root@localhost 14_thread]# 
*
*		
*
*
*
*****************************************************/
#include  <stdio.h>
#include  <pthread.h>

#define	NUM	5

int     counter = 0;

main()
{
	pthread_t t1;			/* one thread */
	void	  *print_count(void *); /* its function */
	int       i;

	pthread_create(&t1, NULL, print_count, NULL);

    /* grb added 2017/07/06 主线程会对全局变量增5 */
	for( i = 0 ; i<NUM ; i++ ){
		counter++;
		sleep(1);
	}

	pthread_join(t1, NULL);
}

/* 子线程打印全局变量counter,而counter被父进程递增了 */
void *print_count(void *m)
{
	int i;
	for(i=0 ; i<NUM ; i++){
		printf("count = %d\n", counter);
		sleep(1);
	}
	return NULL;
}

