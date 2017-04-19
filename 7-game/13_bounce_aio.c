/*
 * 使用标准的posix实现异步输入
 * * bounce_aio.c
 *  *	purpose	animation with user control, using aio_read() etc
 *   *	note	set_ticker() sends SIGALRM, handler does animation
 *    *		keyboard sends SIGIO, main only calls pause()
 *
 *     注意，这个编译要连接上rt库	compile cc bounce_aio.c set_ticker.c -lrt  -lcurses -o bounce_aio
 *
 *
 */

#include<stdio.h>
#include<curses.h>
#include<signal.h>
#include<aio.h>

#define	MESSAGE "hello"
#define	BLANK   "     "

void on_alarm();
void setup_aio_buffer();
void on_input();

int row = 10;
int col =  0;
int dir =  1;
int delay= 200;
int done = 0;


// aiocb 类型的结构体 an aio control buf
struct aiocb kbcbuf;

int main()
{
	initscr();
	crmode();
	noecho();
	clear();
	
	//what new  关键是这三个函数
	signal(SIGIO,on_input);	
	setup_aio_buffer();
	
	// 类似于getch ,进行读，它不会阻塞，并且当收到读后，会发送消息通知你。
	// 发送什么消息，取决于你的设置 kbcbuf.aio_sigevent.sigev_signo = SIGIO;
	aio_read(&kbcbuf);	//请求异步读操作
	
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
	/*
 	*	aio_error(): 检查读是否完成，是否出错
 	* 	This function returns EINPROGRESS if the request has not been  completed  yet.   It  returns  ECANCELED  if  the
 	*   request  was  canceled.   It  returns  0  if  the  request  completed successfully.
 	*
 	*/ 	
	/* 错误检查 */
	if(aio_error(&kbcbuf) != 0)	
		perror("reading fialed");
	// 读完成
	else
		// 获得完成的异步请求的返回状态,跟read系统调用一样，成功返回读到的字节数，失败返回-1
		if(aio_return(&kbcbuf) == 1)
		{
			// 大概是aio_return函数得到输入
			c = *cp;	// 因为cp是指向kbcbuf.aio_buf的，所以aio_buf是存放得到的值
			if( c == 'Q' || c == EOF)
				done  = 1;
			else if (c == ' ')
				dir = -dir ;
		}
	
	// 注意这里还有这个设置 ,继续发读请求
	aio_read(&kbcbuf);
}
void on_alarm()
{
	signal(SIGALRM, on_alarm);	
	mvaddstr(row,col,BLANK );	
	col += dir;			
	mvaddstr(row,col,MESSAGE );	
	refresh();			
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if (dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
}

void setup_aio_buffer()
{
	static char input[1];
	
	kbcbuf.aio_fildes = 0 ;		// fd = 0 ,读标准输入
	kbcbuf.aio_buf = input ;	
	kbcbuf.aio_nbytes = 1 ;
	kbcbuf.aio_offset = 0 ;
	
	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	// 当输入发生时产生SIGIO信号.
	kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}
		

	




