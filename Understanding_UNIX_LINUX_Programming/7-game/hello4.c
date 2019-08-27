/************************************************************
 * 	name	:	hello4.c
 *
 * 	author	:	renbin.guo	
 *
 * 	brief	:	shwo how to user erase,time,and draw for animation
 * 			(画一个后，又擦除)
 *
 * 	history	:	2017-04-15	renbin.guo created
 *
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
			sleep(1);	// hello3.c added
			refresh();	// hello3.c added

			/*hello4.c added*/
			move(i,i+1);
			addstr("           ");	/* erase line */
		}
	//sleep3.c commited
	// refresh();	/* 开始绘画*/
	//getch();
	endwin();
}
		
	
			
	
	
	


