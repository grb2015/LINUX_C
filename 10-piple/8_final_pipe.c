/**********************************************************
 * brief:  一个通用的pipe程序，实现who|sort,它使用两个程序的名字作参数
 * 	   如：pipe who sort  // 将who的输出作为sort的输入
 * 	       pipe ls head
 *
 * history: 2017-05-01 renbin.guo created	 
 *
 * note	:   fork只有，子进程会复制pipe[2]的两个文件描述符pipe[0],pipe[1]
 *	    父进程的pipe[0]和子进程的pipe[0]都执行管道的读端...关掉子进程的pipe[0]并不影响 	 
 *	    父进程的pipe[0],子进程亦然。 具体理解见图10.20
 *
 **********************************************************/

#include<stdio.h>
#include<unistd.h>

#define oops(m,x) {perror(m);exit(x);}
int main(int argc,char **av)
{
	// 管道实际上就是2个fd,所以thepipe[0,1]即使返回的fd
	int thepipe[2];  /*two file fd*/
	int newfd;
	int pid;
	
	// 参数检查
	if(argc != 3)
	{
		fprintf(stderr,"usage: pipe cmd1 cmd2\n");
		exit(1);
	}

	// 创建管道
	if( pipe(thepipe) == -1)
		oops("cannot get a pipe",1);

	// fork子进程
	if( ( pid = fork()) == -1)
		oops("cannot fork",2);
	
	// 父进程的处理: 处理sort,从管道的读端得到数据(本来是从标准输入中得到,这里stdin被指向了管道的读端),进行排序
	if(pid > 0)
	{	// parent will exec av[2] and doesn't write to pipe
		close(thepipe[1]);

		// 将标准输入指向管道的fd pipe[0]
		if( dup2(thepipe[0],0) == -1)
			oops("coult not redirect stdin",3);
		// 由于现在标准输入已经指向了管道的,所以原来的描述符可
		// 可以关了
		close(thepipe[0]);

		// 执行argv[2],注意这里是argv[2],而不是1,所以是sort
		execlp(av[2],av[2],NULL);
	}
	
	/* 子进程的处理	:处理who, 将输出结果写到管道中(通过重定向标准输出到管道的写端实现)*/
	
		
	close(thepipe[0]); // child doesn't read from pipe

	// 将子进程的标准输出重定向到thepipe[1],管道的写端
	if( dup2(thepipe[1],1) == -1)
		oops("count not redirct steout",4);
	close(thepipe[1]);
	execlp(av[1],av[1],NULL);
	oops(av[1],5);
	
}
	
		
		
		
	
	
	
