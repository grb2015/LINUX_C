/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			sem_P_V.c
*   breif:			对一个信号集中的某个信号进行操作的P,V函数
*   history：		2017-06-04renbin.guo created
*   note：		
*   				信号量的值:
*   				1.大于0
*   						表示当前可用资源的数量
*   				2.小于0
*   						表示等待使用该资源的进程数量
*
*   				信号量的值只能由PV操作来改变
*   				
*   				semop用于PV操作
*
*					struct sembuf 
*					{
							unsigned short  sem_num;	// semaphore index in array 
							short		sem_op;			// semaphore operation 
							short		sem_flg;		// operation flags 
					};


*   usage：		
*
================================================================*/
/* P操作 */
int sem_p (int 	semid,	int index)
{
		/* 1.定义一个sembuf结构的对象buf,注意第二个成员很重要! */

			/* 注意,第一个成员为semid,第二个为sops
			 *	 sops: 
			 *	 大于0：信号加上sem_op的值,表示进程释放控制的资源
			 *	 小于0：信号加上sem_op的值,若没有设置IPC_NOWAIT,则
			 *	 		调用进程阻塞,直到资源可用，否则进程直接返回
			 *	 		EAGAIN(eagain)
			 *	 		
			 */
		
		struct sembuf buf = {0,-1,IPC_NOWAIT};
		if(index < 0)
				error_handler("index invaid\n");

		/* 2.通过传入的参数填充sembuf buf */
		buf.sem_num = index;

		/* 3.调用semop进行P操作,1代表要进行操作的信号个数为1个 */
		if( semop(semid,&buf,1) == -1)
				error_handler("semop erorr!\n");
		return 0;	
		
}


int sem_v(int semid	,int index)
{
		/* 第二个参数为1了，代表释放资源 ,就这一个区别*/
		struct	sembuf	buf = {0,1,IPC_NOWAIT};

		if(index < 0)
				error_handler("index invaid\n");

		buf.sem_num = index;

		if( semop(semid,&buf,1) == -1)
				error_handler("semop erorr!\n");
		return 0;	
		
		
}
