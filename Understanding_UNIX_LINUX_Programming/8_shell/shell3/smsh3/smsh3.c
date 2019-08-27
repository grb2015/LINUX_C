 /***********************************************
 * version 3
 *      增加的功能：
 *              允许程序使用变量
 *   modified    process.c  to process2.c
 *   added file  builin.c  varlib.c varlib.h
 *
 *  name: smsh3.c  ---本文与smsh2.c是一样的
 *  bug:
 *	
grb-sim@xxx:~/github/LINUX_CMD/8_shell/shell3/smsh3$ ./sh3 
> set
cannot execute commnad: No such file or directory
*** glibc detected *** ./sh3: double free or corruption (fasttop): 0x0000000000632030 ***
======= Backtrace: =========
/lib/x86_64-linux-gnu/libc.so.6(+0x7da26)[0x7f86dc18ca26]
./sh3[0x401484]
./sh3[0x4014a1]
./sh3[0x4010ff]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xed)[0x7f86dc1307ed]
./sh3[0x4009a9]
======= Memory map: ========
00400000-00402000 r-xp 00000000 08:06 14163232                           /home/grb-sim/github/LINUX_CMD/8_shell/shell3/smsh3/sh3
00601000-00602000 r--p 00001000 08:06 14163232                           /home/grb-sim/github/LINUX_CMD/8_shell/shell3/smsh3/sh3
00602000-00603000 rw-p 00002000 08:06 14163232   


grb-sim@xxx:~/github/LINUX_CMD/8_shell/shell3/smsh3$ ./sh3 
> day=monday
cannot execute commnad: No such file or directory
*** glibc detected *** ./sh3: double free or corruption (fasttop): 0x00000000006a3030 ***
======= Backtrace: =========
/lib/x86_64-linux-gnu/libc.so.6(+0x7da26)[0x7f5dddc95a26]
./sh3[0x401484]
./sh3[0x4014a1]
./sh3[0x4010ff]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xed)[0x7f5dddc397ed]
./sh3[0x4009a9]
======= Memory map: ========
00400000-00402000 r-xp 00000000 08:06 14163232                           /home/grb-sim/github/LINUX_CMD/8_shell/shell3/smsh3/sh3
00601000-00602000 r--p 00001000 08:06 14163232                           /home/grb-sim/github/LINUX_CMD/8_shell/shell3/smsh3/sh3
00602000-00603000 rw-p 00002000 08:06 14163232                           /home/grb-sim/github/LINUX_CMD/8_shell/shell3/smsh3/sh3
0069f000-006c2000 rw-p 00000000 00:00 0                                  [heap]
7f5ddda02000-7f5ddda17000 r-xp 00000000 08:06 24121189                   /lib/x86_64-linux-gnu/libgcc_s.so.1

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

	
