/************************************
 *	filename:	play0.c
 *	author	:	renbin.guo	
 *	brief	：	ask if user wants another transaction
 *			(ATM程序，询问用户是否继续交易)
 *	history	:	2017-04-10	renbin.guo created
 *
 * 	note	:	不足之处 
 * 				 只有按下enter后程序才能接收到输入
 * 				 按行缓冲，只有按下enter键后，才能接收输入，那我按下ahihegy再按enter,返回是什么呢？
 * 				 它只取最后一个字母，所以返回的是y
 * 	
 ************************************/
#include<stdio.h>
int main()
{
	int respanse;
	respanse = get_respanse("Do you want another transaction");
	return respanse;
}
/*****************************************
 *	function:	get_respanse 
 *	auther	:	renbin.guo
 *	breif	:	ask a question and wait for a y/n anser
 *	param	:	问题提示，需要问的问题.
 *	returns	: 	0=>yes , 1=>no
 *	note 	:
 *			ignore non y/n answers
 *			(如果输入不为‘y' 或 'n‘ ,则忽略，不响应，直到输入y/n)
 *
 ****************************************/
int get_respanse(char *question)
{
	printf("%s,(y/n?\n",question);
	/* 一直输入，除非遇到y或者n */
	while(1)
	{
	    switch(getchar())
	    {
		case 'y':
		case 'Y':
			return 0;
		case 'n':
		case 'N':
		case EOF:
			return 1;
	    }
	}
}
		
		
