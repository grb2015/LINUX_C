/*	
 *cp1.c
 *	version 1 of cp --users read and write with runable buffer size
 *	 
 *	usage: cp1 src_file  dest_file
 *
 * 
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFERSIZE 	4096		/*每次读写的长度*/
#define COPYMODE	0644

void oops(char *,char *);
int main(int ac,char *av[])
{
	int 	in_fd,out_fd,n_chars;
	char 	buf[BUFFERSIZE];
	struct	stat sb;
	if(ac != 3)
	{
		fprintf(stderr,"usage: %s source destination.\n",*av);
		exit(1);
	}
 	// 如果原文件与目标文件相同 
	if(strcmp(av[1],av[2])==0)
	{
		fprintf(stderr,"cp:'%s' and '%s' are the same file.\n",av[1],av[2]);		
		exit(1);
	}
		/*open files*/

	if((in_fd = open(av[1],O_RDONLY)) == -1)
		oops("can't  open ",av[1]);
	if( (out_fd = creat(av[2],COPYMODE)) == -1)
		oops("can't creat ",av[2]);
	
		/*copy files*/
	while( (n_chars = read(in_fd,buf,BUFFERSIZE)) > 0)
		if(write(out_fd,buf,n_chars) != n_chars)
			oops("write error to ",av[2]);
	if(n_chars == -1)
		oops("raed error from ",av[1]);
	
		/*close file*/
	if( close(in_fd) == -1 || close(out_fd) == -1)
		oops("error close files","");
}
void oops(char *s1,char *s2)	
{
	fprintf(stderr,"errror:%s",s1);
	perror(s2);
	exit(1);	
}
