/************************************************************
 * 	name	:	hello5.c
 *
 * 	author	:	renbin.guo	
 *
 * 	brief	:	bounce a message back and forth across the screen	
 * 			字符串从左向右移动，碰到右边界后又向左移动，如此往复。
 *
 * 	history	:	2017-04-15	renbin.guo created
 *
 * 	note	:
 * 			usage:	gcc xxx.c -o xxx -lcurses
 *
 *************************************************************/
#include<stdio.h>
#include<curses.h>	 /*  /usr/include/curses.h  */

#define		ROW		10
#define		RIGHTEDGE	30
#define		LEFTEDGE	10

int main()
{
	
	char message[] = "hello5";
	char blank[]   = "      ";
	int dir = +1;
	int pos = LEFTEDGE;

	initscr();
	clear();	
		while(1)
		{
			move(ROW,pos);		/*move to (10,10)*/
			addstr(message);
			move(LINES-1,COLS-1);	/* 移动到坐标顶角 这句是什么作用 ?*/
			refresh();
			sleep(1);
			move(ROW,pos);		/*擦掉刚才画的*/
			addstr(blank);
			pos += dir;
			if(pos >= RIGHTEDGE)	/* 碰到右边界了，则向左移动*/
				dir = -1;
			
			if(pos <= LEFTEDGE)	/* 碰到左边界了，则向右移动*/
				dir = +1;
		}
	//endwin();	/*不需要了 ？因为是while(1),改动画会一直运行*/
}
		
	
			
	
	
	


