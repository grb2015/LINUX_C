/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			write.c
*   breif:			使用共享内存实现进程间通信。读者写者问题
*   				共享内存需要信号量来保护其互斥访问
*   history：		2017-06-06renbin.guo created
*   note：		
*   usage：		
*   				[root@grb_host 7_shared_memery]# ./write 
*   				writer:ddd
*   				writer:ooo
*
*   				[root@grb_host 7_shared_memery]# ./read 
*   				reader:ddd
*   				reader:ooo
*
*   				注意，要等write打印出了"write:" 再写,此时才说明
*   				write得到了共享内存
*
*
*
*
b
================================================================*/
#include"sharemem.h"


#define		SHM_SIZE	1024
int main(int argc, char *argv[])
{
			
		int		semid;
		int		shmid;
		char	*shmaddr;
		char	write_str[SHM_SIZE];

		/* 1. 创建共享内存 */
		shmid = createshm(".",'m',SHM_SIZE);
		if(shmid == -1)
				exit(1);

		/* 2.将共享内存绑定到当前线程 */
		shmaddr = shmat(shmid,(char*)0,0);
		if(shmaddr == (char*)-1)
			error_handler("attach shared memory failed\n");

		/* 3.创建信号量 */
		semid = createsem(".",'s',1,1);
		if(semid == -1)
			exit(1);
		/* 
		 * 4.主流程,一个死循环.不断重复下列动作	
		 *
		 * 	等待共享内存
		 * 	使用共享内存
		 * 	释放共享内存
		 *
		 *
		 */
		while(1)
		{
			wait_sem(semid,0);	/* 等待信号量变为1,可能阻塞 */
			sem_p(semid,0);		/* P操作，将信号量减为0 */

			/* 至此获得了共享内存,开始使用它 */
			printf("writer:");
			fgets(write_str,1024,stdin);
			int 	len = strlen(write_str)-1;
			write_str[len] = '\0';
			strcpy(shmaddr,write_str);
			sleep(10);		/* 使reader处于阻塞状态 */
			sem_v(semid,0);		/* V操作，使信号量变为1 */
			sleep(10);		/* 不要去和reader抢信号量 */
		
		}
}
		



