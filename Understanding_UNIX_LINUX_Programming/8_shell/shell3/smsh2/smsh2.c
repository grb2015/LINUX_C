/********************************************************************
 *    smsh2.c - small-shell version 2
 *         small shell that supports comand line parsing 
 *          and if ..then.. else .fi ligic (by calling process)
 *     
 *     name : smsh2.c
 *     brief: small-shell 第二个版本：
 *     	      主要改进： 支持if..then..else 等条件语句
 *        
 *     history: 2017-04-28 renbin.guo created
 *     note :
 *     	     usage:
 *
 *     	     	gcc *.c -o sh2
 *
 *     	    1.----------------这是标准的shell ,执行正确--------------------
 *     	     [root@ smsh2]# if grep lp /etc/passwd
 *     	     > then
 *     	     > echo ok
 *     	     > fi
 *     	     lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
 *     	     ok
 *     	     [root@ smsh2]# if grep pati /etc/passwd
 *     	     > then
 *     	     > echo ok1
 *     	     > fi
 *     	     [root@ smsh2]# 
 *
 *     	     2.--------为什么执行下面的就会出错----------------- 
 *     	    [root@ smsh2]# ./sh2 
 *     	    > if grep lp /etc/passwd
 *     	    lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
 *     	    Segmentation fault (core dumped)
 *     	    [root@ smsh2]# 
 *
 *     	    bug2 :还是跟verison1 一样，输入一行按下回车就报错，core dumped
 *
 *
 *****************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include "smsh.h"

void setup();
#define	 DFL_PROMPT "> "
int main()
{
	char *cmdline;
	char *prompt;
	char **arglist;
	int result;

	prompt = DFL_PROMPT;
	setup();
	
	
	while( (cmdline = next_cmd(prompt,stdin)) != NULL) 
	{
		if( (arglist = splitline(cmdline)) != NULL) 
		{
			// result = execute(arglist); 	// in version1
			result = process(arglist);	// version 2 modified 
			freelist(arglist);		
		}
		free(cmdline);
	}
	return 0;
}

void setup()
{
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
}

	
