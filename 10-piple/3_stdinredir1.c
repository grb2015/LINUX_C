/* 利用 "最低可用文件描述符"原则
 * show how  to redirect standin by replacing file

  usage:
grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ./std 
hhh
--hhh
hhh
--hhh
hhh
--hhh
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
grb-sim@xxx:~/github/LINUX_CMD/10-piple$

*/

#include<fcntl.h>
#include<stdio.h>

int main()
{
	int fd;
	char line[100];
	
	fgets(line ,100,stdin);
	printf("--%s",line );

	fgets(line ,100,stdin);
	printf("--%s",line );

	fgets(line ,100,stdin);
	printf("--%s",line );

	close(0);	// 关闭了标准输入,之前标准输入时定向到键盘

	fd = open("/etc/passwd",O_RDONLY);	 // fd 0是空闲的，所以文件/etc/passwd被定为到了标准输入
	if(fd != 0)
	{
		fprintf(stderr,"could not open data as fd 0\n");
		exit(0);
	}
	
	fgets(line ,100,stdin);
	printf("%s",line );

	fgets(line ,100,stdin);
	printf("%s",line );

	fgets(line ,100,stdin);
	printf("%s",line );
}
	
		
		
	
