/*
	porpuse	animation with user control, using O_ASYNC on fd
 *	note	set_ticker() sends SIGALRM, handler does animation
 *		keyboard sends SIGIO, main only calls pause()
 *	compile	cc bounce_async.c set_ticker.c -lcurses -o bounce_async
 *	
 *	疑问：	这个程序与之前的10_bounce1.c并没有任何优势啊，不都是用的getch吗，这个只不过是输入放到了一个信号处理函数里面而已
 */
#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<fcntl.h>

/* The state of the game */

#define	MESSAGE	"hello"
#define	BLANK	"     "

int	row   = 10;	/* current row		*/
int	col   =  0;	/* current column	*/
int	dir   =  1;	/* where we are going	*/
int	delay = 200;	/* how long to wait	*/
int	done  = 0;

main()
{
	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	void	enable_kbd_signals();

	initscr();		/* set up screen */
	crmode();
	noecho();
	clear();

    // 设置SIGIO的处理函数为on_input 
	signal(SIGIO, on_input);          /* install a handler        */

	// 允许键盘信号
	enable_kbd_signals();             /* turn on kbd signals      */

	//SIGALRM处理函数为on_alarm
	signal(SIGALRM, on_alarm);        /* install alarm handler    */

	//启动定时器
	set_ticker(delay);		  /* start ticking	      */

	move(row,col);		          /* get into position	      */
	addstr( MESSAGE );	          /* draw initial image       */

	while( !done )			  /* the main loop */
		pause();
	endwin();
}
void on_input(int signum)
{

    // getch非阻塞
	int	c = getch();		  /* grab the char */

	if ( c == 'Q' || c == EOF )
		done = 1;
	else if ( c == ' ' )
		dir = -dir;
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr( row, col, BLANK );	/* note mvaddstr()	*/
	col += dir;			/* move to new column	*/
	mvaddstr( row, col, MESSAGE );	/* redo message		*/
	refresh();			/* and show it		*/

	/*
	 * now handle borders
	 */
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if ( dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
}
/*
 * install a handler, tell kernel who to notify on input, enable signals
 */
 // 设置fd 0 为异步输入
void enable_kbd_signals()
{
	int  fd_flags;

	fcntl(0, F_SETOWN, getpid());       // 设置fd 0 的属主为当前进程
	fd_flags = fcntl(0, F_GETFL);       // // 先获取当前的flags
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));  // 设置新的flags,加上O_ASYNC表示异步输入
}

