/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			read.c
*   breif:			使用共享内存，实现连个进程读者和写者进行通信.
*   				在通信的同时，使用信号量来对共享内存实现互斥访问
*   				(相当于加锁和解锁)
*
*   history：		2017-06-05renbin.guo created
*
*   note：			一个进程读共享内容的时候，其它进程不能写，一个进程写共享内存的时候，其它进程不能读.
*   				实现：两进程read和write在进入共享内存区之前，都先检查信号集中的信号值是否为1,如果不为1，则等待。进入共享内存区之后，将信号量的值减1(相当于加锁),在退出共享内存时，将信号量的值加1（相当于解锁)
*   usage：		
*
================================================================*/
#include <sharemem.h>

int main(int argc, char *argv[])
{
		int 	semid;		/* 信号量id */
		int		shmid;		/* 共享内存id */

		char	*shmaddr;	
		char	write_str[SHM_SIZE];
		/* 创建共享内存，createshm时自定义的函数 */
		shmid = createshm(".",'m',SHM_SIZE) ;
		if(shmid == -1)
				exit(1);
		/* 
		 *
		 *  void *shmat(int shmid, const void *shmaddr, int shmflg);
		 *
		 *  注意下面的(char*)0和(char *)-1
		 *  	
		 *  		第二个参数我们本来打算给它传一个NULL,即地址0x00
		 *  		所以我们把0转为了一个地址。并且指定它为(char*)类型的
		 *  		地址，也就是说，它只占用0x00当前这一个内存单元。如果
		 *  		用(int *)0 ,我们传入的地址仍然是0x00，但是对他占用的					却是以0x00开始,连续的四个内存单元了。
		 *
		 *  		同样,(char *)-1 -1的二进程为0xFF,而这个内存时不能写的，所以代表出错 
		 *
		 *
		 *
		 *
		 * */

		/*  将共享内存与当前进程关联*/
		shmaddr = shmat(shmid,(char*)0,0);
		if ( shmaddr == (char*)-1)
		{
				perror("attach sharemem failed");
				exit(1);
		}
		semid = opensem(".",'s');
		if( semid == -1)
				exit(1);
		while(1)
		{
				printf("reader:");
				wait_sem(semid,0);	  //等待信号量变为1
				
				/* 获得信号量(锁) */
				sem_p(sendid,0);	 // 将信号量-1，是它重新变为0，相当于加锁

				/* 访问共享内存 */
				printf("%s\n",shmaddr);
				sleep(10);
				/* 解锁 */
				sem_v(semid,0);		
				/* 等待write进行写操作 */
				sleep(10);
		}

}


