/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：7_checkerrno.c
*   breif：
*   history：renbin.guo created2017-05-27
*   note：
*   usage：
*   	// 当前目录下我没有test文件
*   	[root@grb_host 8_thread]# ./7 
*   	open file test failed,errno is 2
*   	[root@grb_host 8_thread]# 
*
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
		FILE *stream;
		char *filename = "test";

		errno = 0;		/* 这里error是一个全局变量，所以直接使用，不需要extern ,上面的.h中应该有extern 或者定义 */

		
		stream = fopen(filename,"r");
		if (stream == NULL)
		{
				printf("open file %s failed,errno is %d\n",filename,errno);
		}
		else	
			printf("open file %s successful\n",filename);
}

