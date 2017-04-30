/*  brief:  shows two more methods for redirecting standard input
 *  	    use #define to set one or the other 
 *	    重定位标准输入version2 (使用dup)
 *
 *  histroy：　2017-04-29 renbin.guo created
 */

#include<stdio.h>
#include<fcntl.h>

//  #define CLOSE_DUP
// #define USE_DUP2

int main()
{
	int fd;
	int newfd;
	char line[100];
	
	fgets(line,100,stdin);
	printf("--%s",line);

	fgets(line,100,stdin);
	printf("--%s",line);

	fgets(line,100,stdin);
	printf("--%s",line);

	fd = open("data",O_RDONLY);
#ifdef CLOSE_DUP
	close(0);
	// dup: 复制文件描述符fd为newfd,使fd和newfd都指向同一文件
	newfd = dup(fd);
#else
	// dup2(fd,0) 复制文件描述符fd为0,使fd和0都指向同一文件,返回值
	// newfd仍然为复制成功后的fd 
	newfd = dup2(fd,0);
#endif
	if( newfd != 0)
	{
		fprintf(stderr,"could not duplicate fd to 0\n");
		exit(1);
	}
	close(fd);	 // 关闭fd,使文件只有一个fd指向它,即fd=0
	
	fgets(line,100,stdin);
	printf("%s",line);
	fgets(line,100,stdin);
	printf("%s",line);
	fgets(line,100,stdin);
	printf("%s",line);
}
	
	
	
	
	
