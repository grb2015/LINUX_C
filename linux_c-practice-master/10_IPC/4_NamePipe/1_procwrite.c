/**
 * @file procwrite.c
 * @brief 	¿¿¿¿¿¿¿,¿¿¿¿¿¿¿¿¿¿
 *
 * @author renbin.guo
 * @version 1
 * @date 2017-06-03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME	"myfifo"
#define BUF_SIZE	1024

int main(void)
{
	int     fd;
	char	buf[BUF_SIZE] = "Hello procwrite, I come from process named procread!";

	umask(0);
	if (mkfifo (FIFO_NAME, S_IFIFO | 0666) == -1)
	{
		perror ("mkfifo error!");
		exit (1);
	}

	if((fd = open (FIFO_NAME, O_WRONLY) ) == -1)/*ÒÔÐ´·½Ê½´ò¿ªFIFO*/
	{
		perror ("fopen error!");
		exit (1);
	}
	write (fd, buf, strlen(buf)+1); /*ÏòFIFOÐ´Êý¾Ý*/

	close (fd);
	exit (0);
}
