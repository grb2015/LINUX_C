/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：8_error_show.c
*   breif：
*   history：renbin.guo created2017-05-31
*   note：
*   usage：
*
================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

/**
 * @brief   打开一个文件，如果出错，给出出错原因
 *
 * @param filename
 *
 * @return   通过strerror(errno)实现打印出错字符串,或使用perror
 */
FILE * open_file(char *filename)
{
	FILE *stream;
	errno = 0;

	stream = fopen(filename,"r");
	if (stream == NULL)
	{
			//printf("cannot open the file %s:%s\n",filename,strerror(errno));
			perror("open");
			exit(-1);
	}
	else	
	{
			printf("open successful!");
			return stream;
	}
}
int main(int argc, char *argv[])
{
		char *filename = "test";
		open_file(filename);
		return 0;
		
}


