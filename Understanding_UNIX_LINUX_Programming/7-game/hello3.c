/************************************************************
 * 	name	:	hello3.c
 * 	author	:	renbin.guo	
 * 	brief	:	using refresh and sleep for animated effects(动画效果）
 * 			outline  init draw stuff(画一些东西) ,wrap up
 * 	history	:	2017-04-15	renbin.guo created
 * 	note	:
 * 			usage:	gcc hello3.c -o hello3 -lcurses
 *
 *************************************************************/

#include<stdio.h>
#include<curses.h>	 /*  /usr/include/curses.h  */

int main()
{
	
	int i;
	initscr();
	clear();	
		for(i=0;i<LINES;i++)	
		{
			move(i,i+1);	/* y = x+1*/
			if(i%2 == 1)
				standout();		/* 使屏幕反色*/
			addstr("hello,world 3");
			if(i%2 == 1)
				standend();	/*取消屏幕反色*/
			sleep(1);	// sleep3.c added
			refresh();	// sleep3.c added
		}
	//sleep3.c commited
	// refresh();	/* 开始绘画*/
	//getch();
	endwin();
}
		
	
			
	
	
	


