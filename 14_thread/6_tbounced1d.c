/* tbounce1d.c: controlled animation using two threads 
 *	note	one thread handles animation
 *		other thread handles keyboard input
 *	compile	cc tbounce1d.c -lcurses -lpthread -o tbounce1d
 */
/***********************************************************
*
*   brief : 第7章 10_bounce1d.c 动画的多线程版本。
            这个版本中，主线程用于处理键盘的输入，moving_msg用于处理球的移动
            主线程通过全局变量delay和dir来与moving_msg线程进行通信。从而影响moving_msg的移动速度和方向
    histroy : 2017-05-12 renbin.guo created
*
*
**********************************************************/
#include	<stdio.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>

/* shared variables both threads use. These need a mutex. */

#define	MESSAGE	" hello "

int	row;	/* current row		*/
int	col;	/* current column	*/
int	dir;	/* where we are going	*/
int	delay;	/* delay between moves	*/

main()
{
	int	ndelay;		/* new delay		*/
	int	c;		/* user input		*/
	pthread_t msg_thread;	/* a thread		*/
	void	*moving_msg();

	initscr();		/* init curses and tty	*/
	crmode();
	noecho();
	clear();

	row   = 10;		/* start here		*/
	col   = 0;
	dir   = 1;		/* add 1 to row number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */

	if ( pthread_create(&msg_thread,NULL,moving_msg,MESSAGE) ){
		fprintf(stderr,"error creating thread");
		endwin();
		exit(0);
	}

	while(1) {
		ndelay = 0;
		c = getch();
		if ( c == 'Q' ) break;
		if ( c == ' ' ) dir = -dir;
		if ( c == 'f' && delay > 2 ) ndelay = delay/2;
		if ( c == 's' ) ndelay = delay * 2 ;
		if ( ndelay > 0 )
			delay = ndelay ;
	}
	// 它发送终止信号给thread线程，如果成功则返回0，否则为非0值
	pthread_cancel(msg_thread);
	endwin();
}

void *moving_msg(char *msg)
{
	while( 1 ) {
		usleep(delay*1000);	/* sleep a while 	*/
		move( row, col );	/* set cursor position	*/
		addstr( msg );		/* redo message		*/
		refresh();		/* and show it		*/

		/* move to next column and check for bouncing	*/

		col += dir;		/* move to new column	*/

		if ( col <= 0 && dir == -1 )
			dir = 1;
		else if (  col+strlen(msg) >= COLS && dir == 1 )
			dir = -1;

	}
}
