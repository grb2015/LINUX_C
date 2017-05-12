/* tanimate.c: animate several strings using threads, curses, usleep() 
 *
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		shared variables for communication
 *	compile	cc tanimate.c -lcurses -lpthread -o tanimate
 *	to do   needs locks for shared variables
 *	        nice to put screen handling in its own thread
 */
/************************************************************
    breif : 利用多线程实现多重动画。
            可以接受最多10个命令行参数，使每一个
            参数在不同的行上移动，并且有自己独立
            的速度和方向。用户可以通过按键‘0’，‘1’
            等使该行的字符串反向。

    history:2017-05-12 renbin.guo created
    
    note:
            程序有两个不足之处:
                1.主线程会使用共享全局变量dir，所以需要加锁
                2.修改屏幕和光标位置的函数同样也需要
                  互斥保护。
                  考虑这样的情况:
                   第一个线程使用move将光标置于
                   某个位置，第二个线程又将它移
                   到别的地方去，此时如果第一个线程还以为它在原处，向屏幕写字符串，那么就错了
    
************************************************************/
#include	<stdio.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>

#define	MAXMSG	10		/* limit to number of strings	*/
#define	TUNIT   20000		/* timeunits in microseconds */

struct	propset {
		char	*str;	/* the message */
		int	row;	/* the row     */
		int	delay;  /* delay in time units */
		int	dir;	/* +1 or -1	*/
	};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int main(int ac, char *av[])
{
	int	       c;		/* user input		*/
	pthread_t      thrds[MAXMSG];	/* the threads		*/
	struct propset props[MAXMSG];	/* properties of string	*/
	void	       *animate();	/* the function		*/
	int	       num_msg ;	/* number of strings	*/
	int	     i;

	if ( ac == 1 ){
		printf("usage: tanimate string ..\n"); 
		exit(1);
	}

	num_msg = setup(ac-1,av+1,props);

	/* create all the threads */
	for(i=0 ; i<num_msg; i++)   // 创建num_msg个线程，每个线程代表一个动画。调用animate函数，参数为props[i]
		if ( pthread_create(&thrds[i], NULL, animate, &props[i])){
			fprintf(stderr,"error creating thread");
			endwin();
			exit(0);
		}

	/* process user input */

	/*
	       主线程，处理用户输入。主线程通过修改全局变量props[i].dir来与其它线程通信，从而改变其它线程的运动方向
	*/
	while(1) {
		c = getch();
		if ( c == 'Q' ) break;
		if ( c == ' ' )
			for(i=0;i<num_msg;i++)
				props[i].dir = -props[i].dir;       /// 这里为什么不加锁 ?
		if ( c >= '0' && c <= '9' ){
			i = c - '0';
			if ( i < num_msg )
				props[i].dir = -props[i].dir;
		}
	}

	/* cancel all the threads */

	// 主线程与另外的n个线程共用一把锁mx
	pthread_mutex_lock(&mx);
	for (i=0; i<num_msg; i++ )
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

/*
    breif : 设置每个线程动画的参数，运行的行号，方向，速度(delay),运行的形状(string[i])
   
*/
int setup(int nstrings, char *strings[], struct propset props[])
{
	int num_msg = ( nstrings > MAXMSG ? MAXMSG : nstrings );
	int i;

	/* assign rows and velocities to each string */
	srand(getpid());
	for(i=0 ; i<num_msg; i++){
		props[i].str = strings[i];	/* the message	*/
		props[i].row = i;		/* the row	*/
		props[i].delay = 1+(rand()%15);	/* a speed	*/
		props[i].dir = ((rand()%2)?1:-1);	/* +1 or -1	*/
	}

	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
	// 打印提示信息，使用方法
	mvprintw(LINES-1,0,"'Q' to quit, '0'..'%d' to bounce",num_msg-1);

	return num_msg;
}

/* the code that runs in each thread */

/*
    处理一个动画的移动
*/
void *animate(void *arg)
{
	struct propset *info = arg;		/* point to info block	*/
	int	len = strlen(info->str)+2;	/* +2 for padding	*/
	// 初始化列号
	int	col = rand()%(COLS-len-3);	/* space for padding	*/

	while( 1 )
	{
		usleep(info->delay*TUNIT);

        // 所有的线程都共用一把锁，mx 。所以只有拥有锁的线程可以使用。好像这里加锁并不能保护info->str，因为其它地方
        // 可能摸个时刻正在修改info->str，这里是为了保护什么呢?另外，col是每个线程私有的吗?
		pthread_mutex_lock(&mx);	/* only one thread	*/
		   move( info->row, col );	/* can call curses	*/
		   addch(' ');			/* at a the same time	*/
		   addstr( info->str );		/* Since I doubt it is	*/
		   addch(' ');			/* reentrant		*/
		   move(LINES-1,COLS-1);	/* park cursor		*/
		   refresh();			/* and show it		*/
		pthread_mutex_unlock(&mx);	/* done with curses	*/

		/* move item to next column and check for bouncing	*/

		col += info->dir;

		if ( col <= 0 && info->dir == -1 )
			info->dir = 1;
		else if (  col+len >= COLS && info->dir == 1 )
			info->dir = -1;
	}
}
