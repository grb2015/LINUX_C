/*
 *    smsh2.c - small-shell version 2
 *         small shell that supports comand line parsing 
 *          and if ..then.. else .fi ligic (by calling process)
 *     
 *     name : smsh2.c
 *     brief: small-shell 第二个版本：
 *     	      主要改进： 支持if..then..else 等条件语句
 *        
 *     history: 2017-04-28 renbin.guo created
 */

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
	
