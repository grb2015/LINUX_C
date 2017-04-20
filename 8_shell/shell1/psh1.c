/* 带提示符的shell
 *  uage:
 *  	./psh1
 *  	arg[0]? ls
 *  	arg[1]？ -l
 *  	arg[0]? .
 */
#include<stdio.h>
#include<string.h>

#define	MAXARGS	20	// 最多的参数个数
#define	ARGLEN 100	// 每个参数最大的长度

char* makestring(char *buf);
int execute(char **argv);

int main()
{
	char *arglist[MAXARGS+1];
	int numargs;
	char argbuf[ARGLEN];	// 100个字节
	
	numargs = 0;
	while(numargs < MAXARGS)
	{
		printf("arg[%d]?",numargs);
		// 这里的argbuf包含'\0'
		if(fgets(argbuf,ARGLEN,stdin) && *argbuf!='\n')
			arglist[numargs++] = makestring(argbuf);	
		// 输入的第一个字符是回车
		else	
		{
			if(numargs > 0)
			{
				arglist[numargs]=NULL;
				execute(arglist);
				numargs = 0;	// 没有必要置0
			}
		}
	}
	return 0;
}
// 通过给定的参数，调用execvp来执行参数中包含的程序
int execute(char **argv)
{
	execvp(argv[0],argv);
	perror("execvp failed");	//永远不会执行到这里
	exit(1);
}
//malloc 一个缓冲区, 复制buf中的值到该缓冲区
//reuturns 返回该缓冲区的地址
char* makestring(char *buf)
{
	int len = strlen(buf);
	char *p;
	buf[len-1] = '\0';
	
	p = malloc(len+1);
	// 动态内存分配失败
	if( p == NULL)
	{
		fprintf(stderr,"no momory");
		exit(1);
	}
	// 会拷贝'\0'
	strcpy(p,buf);
	return p;
}
	
	
	
	
	
	
		


