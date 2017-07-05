/* shm_tc2.c - time client shared mem ver2 : use semaphores for locking
 * program uses shared memory with key 99
 * program uses semaphore set with key 9900
 */

#include	<stdio.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>

#define	TIME_MEM_KEY	99		/* kind of like a port number */
#define	TIME_SEM_KEY	9900		/* like a filename	      */
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }
union semun { int val ; struct semid_ds *buf ; ushort *array; };

main()
{
	int	seg_id;
	char	*mem_ptr, *ctime();
	long	now;

	int	semset_id;		/* id for semaphore set	*/

	/* create a shared memory segment */

	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, 0777 );
	if ( seg_id == -1 )
		oops("shmget",1);

	/* attach to it and get a pointer to where it attaches */

	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat",2);

	/* connect to semaphore set 9900 with 2 semaphores */

    // 注意这个TIME_SEM_KEY值和服务器里面是一样的,从而来建立联系，这里建立了2个信号量
	semset_id = semget( TIME_SEM_KEY, 2, 0);
	wait_and_lock( semset_id );

	printf("The time, direct from memory: ..%s", mem_ptr);

	release_lock( semset_id );
	shmdt( mem_ptr );		/* detach, but not needed here */
}

/* 
 * build and execute a 2-element action set: 
 *    wait for 0 on n_writers AND increment n_readers
 */
wait_and_lock( int semset_id )
{
    // sem_info定义在这里却没有使用
	union semun   sem_info;         /* some properties      */
	struct sembuf actions[2];	/* action set		*/

    // sem_op为0，wait for zero
	actions[0].sem_num = 1;		/* sem[1] is n_writers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = 0 ;	/* wait for 0		*/

    // sem_op为1，执行V操作，对n_readers增1
	actions[1].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[1].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[1].sem_op  = +1 ;	/* incr n_readers	*/

    // 这里用数组元素1用于wait for zero，是不是意味着要等0wait成功后，才执行数组元素1的V操作
	if ( semop( semset_id, actions, 2) == -1 )
		oops("semop: locking", 10);
}

/*
 * build and execute a 1-element action set:
 *    decrement num_readers
 *  这里actions[0]为什么说操作的就是n_reader信号量呢?为什么不是n_writers ,答:因为这里有对sem_num赋值为0!
 */
release_lock( int semset_id )
{
	union semun   sem_info;         /* some properties      */
	struct sembuf actions[1];	/* action set		*/

    // 对n_readers执行P操作，减1
	actions[0].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = -1 ;	/* decr reader count	*/

	if ( semop( semset_id, actions, 1) == -1 )
		oops("semop: unlocking", 10);
}
