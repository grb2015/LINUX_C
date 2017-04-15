/************************************************************
 * 	name	:	hello2.c
 * 	author	:	renbin.guo	
 * 	brief	:	show how to use curses  functions with a  loop
 * 	history	:	2017-04-15	renbin.guo created
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
			addstr("hello,world 2");
			if(i%2 == 1)
				standend();
		}
	refresh();	/* 开始绘画*/
	getch();
	endwin();
}
		
	
			
	
	
	


