/* 
 *  brief   : to show 重定向IO的是shell 而不是程序
 *  histroy : 2017-04-29 renbin.guo created
 *  usage:
 *  	gcc listargs.c -o list
       1.  # 这里之所以stderr也打印到了屏幕，是因为stderr 默认也是被重定位到屏幕的
 *	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ./list one two
	argv[0]=./list
	argv[1]=one
	argv[2]=two
	this message is sent to stderr.
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ 
	
	  # 注意，这里的>重定位符号并不作为参数,标准错误仍然输出到屏幕
	2. ./list one two >xyz

	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ./list one two >xyz
	this message is sent to stderr.
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ls
	list  listargs.c  watch.sh  xyz
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ cat xyz 
	argv[0]=./list
	argv[1]=one
	argv[2]=two
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ 

	3. 标准错误被重定位到了文件 oops
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ./list  one two > xyz 2>oops
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ ls
	list  listargs.c  oops  watch.sh  xyz
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ cat oops
	this message is sent to stderr.
	grb-sim@xxx:~/github/LINUX_CMD/10-piple$ 
 * 
 */
#include<stdio.h>

int main(int argc,char **argv)
{
	int i = 0 ;
	while(argv[i] != NULL)
	{
		printf("argv[%d]=%s\n",i,argv[i]); //输出到标准输出
		i++;
	}
	fprintf(stderr,"this message is sent to stderr.\n"); //输出到标准错误
}
