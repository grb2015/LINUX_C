/************************************************************
 * 	name	:	hello1.c
 * 	author	:	renbin.guo	
 * 	brief	:	show the minimal calls needed to use curses
 * 	history	:	2017-04-15	renbin.guo created
 *
 *************************************************************/

#include<stdio.h>
#include<curses.h>	 /*  /usr/include/curses.h  */

int main()
{
	initscr();	/* init curses */

	/* send request*/
	clear();	
	move(10,20);
	addstr("Hello ,word !");	/*在坐标(10,20)处打印字符串*/
	move(LINES-1,0);	/* 让光标停留在左下角*/

	refresh();
	getch();
	
	endwin();	// turn off curses
}
	


