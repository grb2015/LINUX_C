/*************************************
 *	brief	:  字符动画，但是用户可以控制方向和速度
 *	histor	:  2017-04-16	renbin.guo created	
 *	note	:  the main program reads keyboard input
 *		   the handler does the animation(动画)
 *
*************************************/

#include<stdio.h>
#include<curses.h>
#include<signal.h>
#include<sys/time.h>

#define BLANK   "     " 
#define	MESSAGE "hello"
void move_msg(int);
int  set_ticker( n_msecs );

int	row;
int	col;
int	dir;



int main()
{
	int	delay;
	int	ndelay;
	int	c;
		
	initscr();

	// curseg.h
	crmode();	
	noecho();

	clear();
	
	row = 10;
	col = 0;
	dir = 1;
	delay = 200;

	move(row,col);	// 移动到(10,0)
	addstr(MESSAGE);

	signal(SIGALRM,move_msg); // 设置定时器的处理函数为move_msg

	/* set_ticker 设置一个200 微秒的定时器 */
	set_ticker(delay);	// renbin.guo added不做错误处理真的好吗?
	if(set_ticker(delay) == -1)	
	{
		printf("set_ticker return errro\n");
		return 1;
	}
	while(1)
	{
		ndelay = 0;
		c = getch();
		
		if(c == 'Q')	// 退出
			break;
		if( c == ' ')	// 反向
			dir = -dir; 
		if( c == 'f' && delay >2) //速度减小一半 
			ndelay = delay/2; 
		if( c == 's')			
			ndelay = delay*2; // 速度增大一倍
		// 疑问，要ndelay干什么
		if( ndelay > 0)
			set_ticker(delay = ndelay);
	}
	endwin();
	return 0;
}
/*********************************************
 * 功能：每隔delay(200)毫秒在dir 方向上移动1个单位重新绘图(实现动画)
 *
 **********************************************/
void move_msg(int signum)	
{
	/*捕鼠器模型，捕鼠器只能用一次，用后要重新设置*/
	signal(SIGALRM,move_msg);
	move(row,col);	
	addstr(BLANK);
	col += dir;
	move(row,col);	
	addstr(MESSAGE);
	refresh();

	/*如果是向左移动，但是已经到了左边界*/
	if(dir == -1 && col <= 0)
		dir = 1;	// 改变移动方向为向右
	// 反之，也改变方向
	else if(dir == 1 && col+strlen(MESSAGE) >= COLS)	
		dir = -1;	
}
/**********************************************************************
 * funtion :  仅仅是一个定时器而已，n_msecs毫秒后，产生SIGALRM信号
 *
 * returns : 
 * 		-1 =>error
 * 		 0 => ok
 ***********************************************************************/
int  set_ticker( n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs;

        n_sec = n_msecs / 1000 ;
        n_usecs = ( n_msecs % 1000 ) * 1000L ;

        new_timeset.it_interval.tv_sec  = n_sec;        /* set reload  */
        new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
        new_timeset.it_value.tv_sec     = n_sec  ;      /* store this   */
        new_timeset.it_value.tv_usec    = n_usecs ;     /* and this     */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

