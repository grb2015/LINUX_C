/*  changeenv.c - shows how to change the environment
 * 		  note : calls "env" to display its new setting
 * uage:	
 * 	gcc changeenv.c -o changeenv
 * 	gcc showenv.c -o env
 * 	./changenv
 *
 * 	[root@localhost changeEnv]# ./changeenv
 * 	TREM=vt100
 * 	HOME=/on/the/range
 * 	[root@localhost changeEnv]# 
 *
 * note:
 * 	renbin.guo added
 * 		execlp()会把执行之前的替换映像之前的程序的environ传递给之后的程序	
 *
 *
 */

#include<stdio.h>
extern char ** environ;	// 全局变量
int main()
{
	char *table[3];
	
	table[0] = "TREM=vt100";
	table[1] = "HOME=/on/the/range";
	table[2] = 0;
	
	environ = table;
	
	execlp("env","env",NULL);	// execlp 的用法是什么，特别是第二个参数 ?
}


