/* hello_single.c - a single threaded hello world program */

// 线程的引入
// 注：执行这个程序需要10s
// 2017-05-09 renbin.guo created  
#include  <stdio.h>
#define	NUM	5

main()
{
	void	print_msg(char *);

	print_msg("hello");
	print_msg("world\n");
}
void print_msg(char *m)
{
	int i;
	for(i=0 ; i<NUM ; i++){
		printf("%s", m);
		fflush(stdout);
		sleep(1);
	}
}

