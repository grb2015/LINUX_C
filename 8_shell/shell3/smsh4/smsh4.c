/*  small shell version4
 *  modifed :  smsh4.c
 *  added:     process2.c
 *  主要的改进：
 * 	 在version3中，
 *	    1.设置了比如TZ=CST6CDT
	      但是并没有生效
	    2.设置day=moday, 但是echo $day却并没有输出
		
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
	extern char **environ;
	VLenviron2table(environ);	//  version4 added
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
}

	
