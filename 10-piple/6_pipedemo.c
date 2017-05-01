/* breif : to show how wo create and use a pipe
 * 
 * history: 2017-04-30 renbin.guo created
 *
 * note: 管道用于解决who|sort这样的问题，将who的输出作为sort的输入
 * 	 函数说明：
 * 		result = pipe(int array[2])
 * 		pipe创建管道并将其两端连接到两个文件描述符，array[0]为
 *      	读数据端的文件描述符，array[1]为写数据端的文件描述符 
 */
#include<stdio.h>
#include<unistd.h>

int main()
{
	int len;
	int i;
	int apipe[2];  // two file descriptors

	char buf[BUFSIZ];
	
	// get a pipe
	if( pipe(apipe) == -1)
	{
		perror("coult not make pipe");
		exit(1);
	}
	printf("got a pipe ,It is file descripters: {%d %d}\n",apipe[0],apipe[1]);
	
	// read from stdin ,write into pipe,read from pipe, print
	// 从键盘输入，重要不为文件结束符,就一直输入
	while(fgets(buf,BUFSIZ,stdin))
	{
		// fgets会以'\0'结尾?  
		/* 向pipe[1]中写入 */
		len = strlen(buf);
		if(write(apipe[1],buf,len) != len)
		{
			perror("writing to pipe");
			break;
		}
		// wipe 擦除原来buf的内容
		for( i = 0; i<len ;i++) 
	        	 buf[i] = 'X';

		/* 从 pipe[0]中读 */
		len = read(apipe[0],buf,BUFSIZ);
		if(len == -1)
		{
			perror("reading from pipe");
			break;
		}
		/* 输出到屏幕 */
		if(write(1,buf,len) != len)
		{
			perror("writing to stdout");
			break;
		}
	}
}
		
		
	
		
	
	
