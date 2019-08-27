/* shm_ts.c : the time server using shared memory, a bizarre application  */
/*
    brief : 之前是用socket实现的日期服务器和客户端，这里使用共享内存
        
    history: 2017-05-13 renbin.guo created

    note :
            usage:
                    1. server

                        [root@grb_host 15_IPC]# ./2_server 

                    2. client
                        [root@grb_host 15_IPC]# ./2_client 
                        The time, direct from memory: ..Sun May 14 21:29:29 2017
                        [root@grb_host 15_IPC]#
            bug:    客户端和服务器可以同时访问共享内存，如果客户端在读的时候，服务器在
                    写，那么得到的就可能是不正确的。解决方法可使用信号量

*/
#include	<stdio.h>
#include	<sys/shm.h>
#include	<time.h>

#define	TIME_MEM_KEY	99			/* like a filename      */
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }

main()
{
	int	    seg_id;
	char	*mem_ptr, *ctime();
	long	now;
	int	    n;

	/* create a shared memory segment */

    // 调用shmget，创建共享内存。返回seg_id. 注意TIME_MEM_KEY，客户端也会用同一个TIME_MEM_KEY
	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT|0777 );
	if ( seg_id == -1 )
		oops("shmget", 1);

	/* attach to it and get a pointer to where it attaches */
    // 以seg_id 作参数，返回一个mem_ptr(分配的共享内存的首地址?)
	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat", 2);

	/* run for a minute */
	//renbin.guo added 显然这个服务器只能精确到秒，它一秒钟才更新一次
	for(n=0; n<60; n++ ){
		time( &now );			/* get the time	*/
		// 将时间日期数据写入mem_ptr    renbin.guo aded 2017/07/05 now应该包含了字符串结尾的'\0',而strcpy会复制'\0'
		strcpy(mem_ptr, ctime(&now));	/* write to mem */
		sleep(1);			/* wait a sec   */
	}
		
	/* now remove it */
	shmctl( seg_id, IPC_RMID, NULL );
}
