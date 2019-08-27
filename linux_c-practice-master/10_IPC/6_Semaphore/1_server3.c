/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			1_server3.c
*   breif:			演示进程间通信：信号量.处理临界资源同步。	
*   				server创建一个信号集，并对信号量循环减1,相当于分配资源	
*   history：		2017-06-04renbin.guo created
*   note：		
*   				
*   			[root@grb_host 6_Semaphore]# ./server3 
*   			0 times sem succussful!
*   			1 times sem succussful!
*   			2 times sem succussful!
*   			3 times sem succussful!
*   			4 times sem succussful!
*   			semop error
*
*   			[root@grb_host 6_Semaphore]# 
*
*
*
================================================================*/

#include <sys/types.h>
#include <linux/sem.h>

#define     MAX_RESOURCE	5

void error_handler(char *s)
{
		printf("%s\n",s);
		exit(1);
}
int main(int argc, char *argv[])
{
		int count = 0;
		key_t	key;
		int		semid;
		struct	sembuf	sbuf = {0,-1,IPC_NOWAIT};
		union	semun	semopts;
		/* 创建一个信号量 */
		key = ftok(".",'s');
		if(key == -1)
				error_handler("ftok error\n");
		/* 1代表只有一个信号量的信号集 */
		semid = semget(key,1,IPC_CREAT|0666);
		if(semid == -1)
				error_handler("semget error\n");

		/* 设置当前信号量数值为5 */
		semopts.val = MAX_RESOURCE;
		/* 0 为index */
		if( semctl(semid,0,SETVAL,semopts) == -1)
		{
			error_handler("semctl\n");
		}

		while(1)
		{

				/* 执行PV操作中的P操作,递减信号量 */
				/* 这是因为前面初始化的时候为-1 */
				//sbuf = {0,-1,IPC_NOWAIT}; 
				if( semop(semid,&sbuf,1) == -1)
					error_handler("semop error\n");
				else	
						printf("%d times sem succussful!\n",count++);
				sleep(3);
		}
		exit(0);
}




				


