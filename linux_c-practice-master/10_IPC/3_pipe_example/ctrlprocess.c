/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			ctrlprocess.c
*   breif:		
*   history：		2017-06-01renbin.guo created
*   note：		
*   usage：		
*
================================================================*/



/**
 * @brief 	从标准输入读取数据(字符串)，然后解析。
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
		int		n;
		char 	buf[1034];

		/* renbin.guo add  begin*/
		int i;
		for (i = 0; i<argc; i++)
		{
				printf("in ctrlprocess : argv[%d] = %s\n",i,argv[i]);
		}
		
		/* renbin.guo add	end*/


		while(1)
		{
				/* 从标准输入读取数据 */
				if ( (n = read(0,buf,1024)) > 0)
				{
						buf[n] = '\0';
						printf("in ctrlprocess : ctrlprocess receive :%s\n",buf);
						if (!strcmp(buf,"exit"))
						{
								printf("in ctrlprocess: going to exit");
								exit(0);
						}
						if (!strcmp(buf,"getpid"))
						{
								printf("in ctrlprocess : my pid :%d\n",getpid());
								sleep(3);
								exit(0);
						}
				}
		}
}
