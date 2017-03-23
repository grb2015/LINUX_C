#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

void show_info(struct utmp*);
void shwo_time(long);
int main()
{
		struct utmp current_record;
		int utmpfd;
		int reclen = sizeof(current_record);
		if(utmpfd = open(UTMP_FILE,O_RDONLY) == -1)
		{
			perror(UTMP_FILE);
			return 0;
		}
		while(read(utmpfd,&current_record,reclen) == reclen)
			show_info(&current_record);
		return 0;
}
void show_info(struct utmp*utbufp)
{
		if(utbufp->ut_type != USER_PROCESS)
			return ;
		printf("% -8.8s",utbufp->ut_user);
		printf("% -8.8s",utbufp->ut_line);
		show_time(utbufp->ut_time);
		printf("%s",utbufp->ut_host);
		printf("\n");
}
void show_time(long ltime)
{
	char *cp;
	cp = ctime(&ltime);
	printf("%12.12s",cp+4);

}
