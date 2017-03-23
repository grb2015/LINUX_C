/***************************************************************
 *	name: 	who.c
 *	auther:	renbin.guo
 *	brief:	self defined linux cmd 'who'
 *	history:	
 *		2017-03-23	renbin.guo added
 *			
 ***************************************************************/
#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

void show_info(struct utmp*);
void show_time(long);		

int main()
{
		struct utmp current_record;
		int utmpfd;
		int reclen = sizeof(current_record);
		if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1)
		{
			perror(UTMP_FILE);
			return 0;
		}
		/*一直读，每次读取一个utmp的结构，长度为reclen，然后显示,知道把所有结构数组都读完*/
		while(read(utmpfd,&current_record,reclen) == reclen)
			show_info(&current_record);
		close(utmpfd);
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
