/* 	brief  ：实现功能 => 让求在上下左右跳动
 *  		实现原理:
		usge:
				user input: 	
					s  slow down x component
				        S: slow y component
 *		 			f  speed up x component
 *		 		        F: speed up component
 *		 	 	        Q quit
 *  	histroy:			
 *  		2016-04-16	renbin.guo created
 *  		2016-04-18	renbin.guo added command 'note'
 *  			
 *  	note   :	
 *  		bug:	疑问：如果我们按键，把y_ttm减为负数了怎么办?貌似是个bug
 *  	
 *  		gcc 11_bounce2d.c set_ticker.c -lcurses -o bounce2d 
 *  		
 *  			
 ****************************************/

#include<stdio.h>
#include<signal.h>
#include<ncurses.h>
#include<sys/time.h>
#include"bounce.h"	

struct	ppball	the_ball;

void  set_up();
void  wrap_up();
void ball_move(int signum);

int main()
{
	int c;
	set_up();	// init_structure and other stuff
	while( (c=getchar()) != 'Q')
	{
		if( c=='f' )
			the_ball.x_ttm--;
		else if( c=='s' )
			the_ball.x_ttm++;
		else if( c=='F' )
			the_ball.y_ttm--;
		else if( c=='S' )
			the_ball.y_ttm++;
	}
	wrap_up();
}


// init_structure and other stuff
void set_up()
{

	/* init the ball */
	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	
	the_ball.y_ttg = Y_TTM;
	the_ball.x_ttg = X_TTM; 

	the_ball.y_ttm = Y_TTM;
	the_ball.x_ttm = X_TTM;

	the_ball.y_dir = 1;	
	the_ball.x_dir = 1;	
	// 球的符号 'O'
	the_ball.symbol = DFL_SYMBOL;
	
	initscr();
	noecho();	//我们输入的字符肯定不能在屏幕显示
	crmode();

	signal(SIGINT,SIG_IGN); //忽略中断信号
	// curses.h
	mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
	refresh();

	/*注册定时器信号处理函数为ball_move控制球的运动*/	
	signal(SIGALRM,ball_move);

	set_ticker(1000/TICKS_PER_SEC);	//启动定时器
}

void wrap_up()
{
	set_ticker(0);
	endwin();
}

/* SIGALRM处理函数，控制球的运动 */
void ball_move(int signum)
{
	int y_cur;
	int x_cur;
	int moved;

	signal(SIGINT,SIG_IGN); //忽略中断信号
	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;
	
	/* 
 	 *  我们来考察什么都不按的情况:
 	 *	初始时，y_ttg=y_ttm=8,显然满足y_ttm>0,但是不满足y_ttg--==1,所以不进入执行，但是y_ttg变为了7了
 	 *	然后一直减，知道y_ttg为2的时候，OK，可以进去执行了，此时图形就可以运动了，同时，y_ttg被赋新值y_ttm = 8
 	 *  再考虑我们按键的情况：
 	 *  	当我们按下'S'时，y_ttm++，变为9，所以下次为y_ttg赋值就为了9，所以要运动，时间就会花的长一点,从而可以控制.
 	 *  	速度.
 	 *  疑问：如果我们按键，把y_ttm减为负数了怎么办?貌似是个bug
 	 *
 	 */
	if ( the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1 ){
		the_ball.y_pos += the_ball.y_dir ;	/* move	*/      // y轴位置是否移动，要看y_ttm变量是否大于0
		the_ball.y_ttg = the_ball.y_ttm  ;	/* reset*/      // 还要看y_ttg是否大于1
		moved = 1;                                          // 如果都满足，则y_pos会增加y_dir.
	}
		                                                    //
	/* 这里只对x方向设置 */
	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1)
	{
		the_ball.x_pos +=  the_ball.x_dir; /* move */
		the_ball.x_ttg +=  the_ball.x_ttm; /** reset */
		moved = 1;
	}
	
	if(moved)
	{
		mvaddch(y_cur,x_cur,BLANK);	// 这里为什么要两次?
		mvaddch(y_cur,x_cur,BLANK);
		mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES-1,COLS-1);
		refresh();
	}
	signal(SIGALRM,ball_move);
}

/* 边界判断 */
int bounce_or_lose(struct ppball *bp)
{
	int return_val = 0;
	if(bp->y_pos == TOP_ROW)
	{
		bp->y_dir = 1;
		return_val = 1;
	}
	if(bp->y_pos == BOT_ROW)
	{
		bp->y_dir = -1;
		return_val = 1;
	}
	if(bp->x_pos == LEFT_EDGE)
	{
		bp->x_dir = 1;
		return_val = 1;
	}
	if(bp->x_pos == RIGHT_EDGE)
	{
		bp->x_dir = -1;
		return_val = 1;
	}
	return return_val;
}
		
		
	
		

	
	
