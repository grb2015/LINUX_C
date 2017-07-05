/*********************************************
*       breif : 开2个线程来执行直线的1_hello_single程序，
*               使得执行时间变为一半
*       history ： 2017-05-09 renbin.guo created
*
*
*       usage:
*               [root@localhost 14_thread]# gcc 2_hello_multi.c -o 2_hello_multi -lpthread
*               [root@localhost 14_thread]# ./2_hello_multi
*               world
*               hello
*               world
*               hello
*               world
*               hello
*               world
*               hello
*               world
*               hello
*               [root@localhost 14_thread]# ./2_hello_multi
*
*
*********************************************/

/* hello_multi.c - a multi-threaded hello world program */

#include  <stdio.h>
#include  <pthread.h>

#define	NUM	5

main()
{
	pthread_t t1, t2;		/* two threads */

	void	*print_msg(void *);

	pthread_create(&t1, NULL, print_msg, (void *)"hello\n");
	pthread_create(&t2, NULL, print_msg, (void *)"world\n");

	// main线程等待t1,t2结束
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
void *print_msg(void *m)
{
	char *cp = (char *) m;
	int i;
	for(i=0 ; i<NUM ; i++){
		printf("%s", m);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}

