/* shm_ts2.c - time server shared mem ver2 : use semaphores for locking
 * program uses shared memory with key 99
 * program uses semaphore set with key 9900
 */
/*  breif :  
          之前的共享内存的日期时间服务器的版本存在竞态问题。
          这里通过信号量来解决.
          读的时候，要等所有的等待写的进程计数为0.
          写的时候，要等所有的等待读的进程计数为0
   history: 2017-05-16 renbin.guo created
*/

#include	<stdio.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/sem.h>
#include	<signal.h>

#define	TIME_MEM_KEY	99			/* like a filename      */
#define	TIME_SEM_KEY	9900
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }

union semun { int val ; struct semid_ds *buf ; ushort *array; };
int	seg_id, semset_id;			/* global for cleanup()	*/
void	cleanup(int);

main()
{
	char	*mem_ptr, *ctime();
	time_t	now;
	int	n;

	/* create a shared memory segment */
    // 获得共享内存 begin
	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT|0777 );
	if ( seg_id == -1 )
		oops("shmget", 1);

	/* attach to it and get a pointer to where it attaches */

	mem_ptr = shmat( seg_id, NULL, 0 );    
	if ( mem_ptr == ( void *) -1 )
		oops("shmat", 2);
    // 获得共享内存 end


    // 创建信号量
	/* create a semset: key 9900, 2 semaphores, and mode rw-rw-rw */

	semset_id = semget( TIME_SEM_KEY, 2, (0666|IPC_CREAT|IPC_EXCL) );
	if ( semset_id == -1 )
		oops("semget", 3);

	set_sem_value( semset_id, 0, 0);	/* set counters	*/      // 设置第一个信号量  readernum
	
	set_sem_value( semset_id, 1, 0);	/* both to zero */         // 设置第二个信号量  readernum

   // 按下ctrl+C
	signal(SIGINT, cleanup);

	/* run for a minute */
	for(n=0; n<60; n++ ){
		time( &now );			/* get the time	*/
				printf("\tshm_ts2 waiting for lock\n");
		wait_and_lock(semset_id);	/* lock memory	*/
				printf("\tshm_ts2 updating memory\n");
		strcpy(mem_ptr, ctime(&now));	/* write to mem */
				sleep(5);
		release_lock(semset_id);	/* unlock	*/
				printf("\tshm_ts2 released lock\n");
		sleep(1);			/* wait a sec   */
	}
		
	cleanup(0);
}

// 删除信号量
void cleanup(int n)
{
	shmctl( seg_id, IPC_RMID, NULL );	/* rm shrd mem	*/          /*释放共享内存 ,这个只需要在服务器中执行即可,而客户程序中用的是shmdt  renbin.guo /2017/07/05*/
	semctl( semset_id, 0, IPC_RMID, NULL);	/* rm sem set	*/      /* 释放信号量 */
}

/*
 * initialize a semaphore
 */
set_sem_value(int semset_id, int semnum, int val)
{
	union semun  initval;

	initval.val = val;
	if ( semctl(semset_id, semnum, SETVAL, initval) == -1 )
		oops("semctl", 4);
}

/* 
 * build and execute a 2-element action set: 
 *    wait for 0 on n_readers AND increment n_writers
 */
wait_and_lock( int semset_id )
{
    // sembuf是系统内部结构体
	struct sembuf actions[2];	/* action set		*/

	actions[0].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = 0 ;	/* wait til no readers	*/

	actions[1].sem_num = 1;		/* sem[1] is n_writers	*/
	actions[1].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[1].sem_op  = +1 ;	/* incr num writers	*/

	if ( semop( semset_id, actions, 2) == -1 )
		oops("semop: locking", 10);
}

/*
 * build and execute a 1-element action set:
 *    decrement num_writers
 */
release_lock( int semset_id )
{
	struct sembuf actions[1];	/* action set		*/

	actions[0].sem_num = 1;		/* sem[0] is n_writerS	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = -1 ;	/* decr writer count	*/

	if ( semop( semset_id, actions, 1) == -1 )
		oops("semop: unlocking", 10);
}
