/*
 * 使用标准的posix实现异步输入
 * * bounce_aio.c
 *  *	purpose	animation with user control, using aio_read() etc
 *   *	note	set_ticker() sends SIGALRM, handler does animation
 *    *		keyboard sends SIGIO, main only calls pause()
 *     *	compile cc bounce_aio.c set_ticker.c -lrt  -lcurses -o bounce_aio
 *
 *
 */

#include<stdio.h>
#include<curses.h>
#include<signal.h>
#include<aio.h>

#define	MESSAGE “hello”
#define	BLANK   “     ”

int row = 10;
int col =  0;
int dir =  1;
int delay= 200;
int done = 0;


// aiocb 类型的结构体 an aio control buf
struct aiocb kbcbuf;

int main()
{
	initsrc();
	crmode();
	noecho();
	clear();
	
	//what new  关键是这三个函数
	signal(SIGIO,on_input);	
	setup_aio_buffer();
	// 类似于getch ,进行读，它不会阻塞，并且当收到读后，会发送消息通知你。
	// 发送什么消息，取决于你的设置 kbcbuf.aio_sigevent.sigev_signo = SIGIO;
	aio_read(&kbcbuf);
	
	signal(SIGALRM,on_alarm);
	set_ticker(delay);

	mvaddstr(row,col,MESSAGE);
	while(!done)
		pause();
	endwin();
}

void on_input()
{
	int c;
	char *cp = kbcbuf.aio_buf;
	/* 错误检查 */
	if(aio_error(&kbcbuf) != 0)
		perror("reading fialed");
	else
		// aio_return 得到输入的字符，然后处理这个字符
		if(aio_return(&kbcbuf) == 1)
		{
			// 大概是aio_return函数得到输入
			c = *cp;
			if( c == 'Q' || c == EOF)
				done  = 1;
			else if (c == ' ')
				dir = -dir ;
		}
	
	// 注意这里还有这个设置
	aio_read(&kbcbuf);
}
void on_alarm()
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr( row, col, BLANK );	/* clear old string	*/
	col += dir;			/* move to new column	*/
	mvaddstr( row, col, MESSAGE );	/* draw new string	*/
	refresh();			/* and show it		*/
	/*
 * 	 * now handle borders
 * 	 	 */
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if ( dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
}

void setup_aio_buffer()
{
	static char input[1];
	
	kbcbuf.aio_fildes = 0 ;
	kbcbuf.aio_buf = input ;
	kbcbuf.aio_nbytes = 1 ;
	kbcbuf.aio_offset = 0 ；
	
	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	// 当输入发生时产生SIGIO信号.
	kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}
		

	




