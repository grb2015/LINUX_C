/************************************
 *
 * 	brief:	测试以不同顺序输入ctrl+c和ctrl+\(产生SIGINT和SIGQUIT信号)
 * 		系统的处理方式
 *
 * 	histroy: 2017-04-16 renbin.guo created
 *
 * 	note:	
 * 		测试用例：
 * 			^c^c^c
 * 			^\^c^\^c
 * 			hello^cworld
 * 			^\^\hello^c
 *
 ***********************************/

#include<stdio.h>
#include<signal.h>
void interup_handler(int s);
void quit_handler(int s);

#define INPUTLEN	100
int main()
{
	char input[INPUTLEN];
	int len;
	signal(SIGINT,interup_handler);
	signal(SIGQUIT,quit_handler);
	len = read(0,input,INPUTLEN-1); // 最后一个字符要存放'\0'
	if(len == -1)
		perror("read");
	input[len] = '\0';
	 // 一直循环，知道你输入"quit".你可以输入任何字符串，当然也可以输入ctrl+c 产生中断信号。
	 // 输入ctrl+\产生QUIT信号。 ctrl+c 和ctrl+\都不会终止循环的。
	while(1)
	{
		if( !strncmp(input,"quit",4))
			break;
		printf("%s\n",input);
		len = read(0,input,INPUTLEN-1); // 最后一个字符要存放'\0'
		if(len == -1)
			perror("read");
		input[len] = '\0';
	}
}
void interup_handler(int s)
{
	printf("received SIGINT begin \n");
		sleep(3);
	printf("received SIGINT end\n");
}
void quit_handler(int s)
{
	
	printf("received SIGQUIT begin\n");
		sleep(3);
	printf("received SIGQUIT end\n");
}

		
		
	
	
