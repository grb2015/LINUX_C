/*********************************************
 *
 * 	filename:	play1.c
 * 	author  :	renbin.guo	
 * 	brief	：	set tty into char-by-char mode,read char,return result
 * 			(对play0.c的改进，现在按一个键就立即返回，不需要按enter)
 * 	history	:	2017-04-10	renbin.guo creted
 * 	note	:	不足之处：	
 * 			  如果输入非y or n，最好是不要打印出来
 * 			 better:  do no echo inappropriate input
 * 				
 *
 *******************************************/
#include<stdio.h>
#include<termios.h>

#define	QUESTION	"Do you want another transaction"

void set_crmode();
void tty_mode(int mode);
int get_response(char * question);

int main()
{
	int response;	
	tty_mode(0);	/* save current tty mode */
	set_crmode();  /*  set chr-by-chr mdoe */
	response = get_response(QUESTION);
	tty_mode(1);
	return response;
}


int get_response(char * question)
{
	int input;
	printf("%s(y/n)\n",question);
	while(1)
	{
		
		switch(input =  getchar() )
		{
			case 'y':
			case 'Y':
				return 0;
			case 'N':
			case 'n':
			case EOF:
				return 1;
			default	:
				printf("\ncannot understand: %c\n",input);
				printf("please input y/n\n");
		}
	}
	
}
	


/*
 *   save or reset termios mode
 *   mode=0  =>save
 *   mode=1  =>reset
 */

void tty_mode(int mode)
{
        static struct termios info_backup;
        if( mode == 0)
		/* 保存获取的标准输入的termios信息 */
                tcgetattr(0,&info_backup);
        else
		/*恢复之前保存的信息,TCSANOW立即生效，*/
                tcsetattr(0,TCSANOW,&info_backup);
}


/*	
 *	set stdin to char-by-char mode
 */
void set_crmode()
{
	struct termios tty_info;
	tcgetattr(0,&tty_info);			/*read curr info*/
	tty_info.c_lflag	&= ~ICANON;	/*no buffering*/
	tty_info.c_cc[VMIN] 	= 1;		/*get 1 char at a time*/	
        tcsetattr(0,TCSANOW,&tty_info);	/*install seting*/
}
	
	
	
