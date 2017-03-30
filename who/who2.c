/***************************************************************
 *	name: 	who.c
 *	auther:	renbin.guo
 *	brief:	self defined linux cmd 'who'
 *	note :
 *		verison 2 : who with bufferd reads 
 *			
 *	history:	
 *		2017-03-29	renbin.guo added
 *	
 *			
 ***************************************************************/
#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

//renbin.guo added 
#include"utmplib.h"

void show_info(struct utmp*);
void show_time(long);		

int main()
{
	/* struct utmp current_record; int utmpfd;
		int reclen = sizeof(current_record);
		if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1)	
		{
			perror(UTMP_FILE);
			return 0;
		}
		while(read(utmpfd,&current_record,reclen) == reclen)
			show_info(&current_record);
		close(utmpfd);
		return 0;
	*/
	struct utmp *utbufp;
	//struct utmp *utmp_next();	//这里为什么要声明这个函数.utmp_open()都没有声明
	
	/*
	*
 	*	打开文件，这里并没有fd返回,fd回在utmp_next里面使用
 	*/
	if(utmp_open(UTMP_FILE) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}

	/*
 	*
	*   一次Load 16 个记录，但每次只返回一个记录来显示
	*    
  	*/

	while( (utbufp= utmp_next()) != (struct umpt*)NULL)
		show_info(utbufp);
	utmp_close();
	return 0;

}
void show_info(struct utmp*utbufp)
{
		if(utbufp->ut_type != USER_PROCESS) 	//我们读出的utmp结构有的并不是当前正在登陆的，所以要筛选
			return ;
		printf("% -8.8s",utbufp->ut_user);
		printf("% -8.8s",utbufp->ut_line);
		show_time(utbufp->ut_time);
    	printf("(%s)", utbufp->ut_host);
		printf("\n");
}
void show_time(long ltime)
{
	char *cp;
	cp = ctime(&ltime);
	printf("%12.12s",cp+4);			//注意，我们并不显示week，只显示日期 所以cp+4，

}
