/* incprint.c - one thread increments, the other prints */
/*****************************************************
*  brief : 线程间通信--共享全局变量
*  history: 2017-05-09 renbin.guo created
*  note :
* 	usage:
*		gcc -o incprint incprint.c  -lpthread
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
	for( i = 0 ; i<NUM ; i++ ){
		counter++;
		sleep(1);
	}

	pthread_join(t1, NULL);
}
void *print_count(void *m)
{
	int i;
	for(i=0 ; i<NUM ; i++){
		printf("count = %d\n", counter);
		sleep(1);
	}
	return NULL;
}

