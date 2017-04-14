/**********************************************
 *   filename     :       play4.c
 *   auther       ：      renbin.guo
 *   brief        ：      set tty into char-by-char and no-echo mode 
		          and no-delay mode,resets terminal modes on SIGINT,ignores SIGQUIT
 *      
 *   note         :       
 *                        对play3的改进，收到中断信号时，先reset之前的模式，然后再退出，免得连会话都退了,
		          同时忽略掉SIGQUIT信号(ctrl+/)
 *   				  
 *                        
 *
 *   history      :       2017-04-14     renbin.guo created
 *           
 ** ***********************************************/

#include	<stdio.h>
#include	<termios.h>
#include	<fcntl.h>
#include	<string.h>
#include	<signal.h>	//renbin.guo add for play4.c

#define	QUESTION	"Do you want another transaction"
#define SLEEPTIME	1	
#define TRIES		7	

void set_nodelay_mode();
int get_response(char *question,int maxtries);
char get_ok_char();
void  ctrl_c_handler(int signum);

int main()
{
	int	response;

	tty_mode(0);				/* save mode */
	set_cr_noecho_mode();			/* set -icanon, -echo	*/
	set_nodelay_mode();		
	signal(SIGINT,ctrl_c_handler);		/*regist SININT handl fuction*/
	signal(SIGQUIT ,SIG_IGN);	/* ignore SIGQUIT signals*/
	response = get_response(QUESTION,TRIES);	/* get some answer	*/
	tty_mode(1);				/* restore tty state	*/
	return response;
}

int get_response(char *question,int maxtries)
/*
 *  * purpose: ask a question and wait for a y/n answer
 *   *  method: use getchar and ignore non y/n answers
 *    * returns: 0=>yes, 1=>no 2=>timeout
 *     */
{
	int input;
	printf("%s (y/n)?", question);
	fflush(stdout);		//这里很重要
	while(1){
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());		//注意，这里的getchar()不会阻塞进行等待输入，所以定时器才起作用
		switch( input ){
			case 'y': 
				 return 0;
			case 'n': 
			case 'EOF':	//renbin.guo added 201-04-12
				return 1;
		}
		if(maxtries-- == 0)
			return 2;
		putchar('\a');	//声音，模拟按键
	}
}
void set_cr_noecho_mode()
/* 
 *  * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *   *  method: use bits in termios
 *    */
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	tcsetattr( 0 , TCSANOW, &ttystate);	/* install settings	*/
}

/* how == 0 => save current mode,  how == 1 => restore mode */
/*    renbin.guo added:   bug:    you must set_mode(0)first before you reset set_mode(1)*/

void tty_mode(int how)

{
	static struct termios original_mode;
	static 	int 	original_flags;
	if ( how == 0 )
	{
		original_flags = fcntl(0,F_GETFL);
		tcgetattr(0, &original_mode);
	}
	else
	{
		tcsetattr(0, TCSANOW, &original_mode); 
		fcntl(0,F_SETFL,original_flags);
	}
}

/*
 *	设置非阻塞输入，设置后，比如getchar(),就不会一直阻塞等待输入了
 */

void set_nodelay_mode()
{
	int 	termflags;
	termflags = fcntl(0,F_GETFL);	/* 读取当前的的设置*/
	termflags |= O_NDELAY;		/* 设置非阻塞*/
	fcntl(0,F_SETFL,termflags);     /* install 新设置*/
}
/*
 *    skip over non-legal chars and return y,Y,n,N or EOF
 */

char get_ok_char()
{
	int c;
	while( (c=getchar()) !=EOF && strchr("yYnN",c) == NULL)
		;
	return c;
}

/*
*	auther:	renbin.guo 2017-04-14 created
	brief : handle SIGINT signals
*
*/
void  ctrl_c_handler(int signum)
{
	tty_mode(1);
	exit(2);
}

	
	
