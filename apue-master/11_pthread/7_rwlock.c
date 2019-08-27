/**
 * @file 7_rwlock.c
 * @brief	使用读写锁，实现对队列的安全访问 
 * @author renbin.guo
 * @version 1
 * @date 2017-06-10
 *
 * note:
 *
 * 		函数原型:  
 * 		 为什么*restrict?不应该直接是pthread_rwlock_t *吗?
 * 		 int pthread_rwlock_init (pthread_rwlock_t *restrict rwlock,
 * 		               const pthread_rwlockattr_t *restrict attr);
 *
 * 		读写锁：
 * 				1.当锁处于写的状态时，所有试图读或写的进程都被阻塞
 * 				2.当锁处于读的状态时，所有试图写的进程都被阻塞,读的进程可以继续。
 * 				
 * 				
 *
 * 		 
 *
 *
 * usage:
 */

#include <stdlib.h>
#include <pthread.h>

/* 链表的节点。有一个prev指针指向链表前一个节点,next指针指向后一个节点
 * 
 * 节点的值为 j_id
 *  */

struct job {
	struct job *j_next;
	struct job *j_prev;
	pthread_t   j_id;   /* tells which thread handles this job */
	/* ... more stuff here ... */
};


/* 队列数据结构,先进先出。显然它有一个队列头指针和尾指针 */
/* 这里的q_lock,用来对队列加锁，控制访问 */
struct queue {
	struct job      *q_head;
	struct job      *q_tail;
	pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue.
 *
 * 队列头指针和尾指针都指向NULL
 */
int
queue_init(struct queue *qp)
{
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;
	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0)
		return(err);
	/* ... continue initialization ... */
	return(0);
}

/*
 * Insert a job at the head of the queue.
 *	插入一个节点到队列头 (注意后面还有append,则是插入到队列尾)
 */
void
job_insert(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);	/* 获得写锁 */
	jp->j_next = qp->q_head;
	jp->j_prev = NULL;
	if (qp->q_head != NULL)
		qp->q_head->j_prev = jp;
	else
		qp->q_tail = jp;	/* list was empty */
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);/* 释放写锁 */
}

/*
 * Append a job on the tail of the queue.
 */
void
job_append(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = NULL;
	jp->j_prev = qp->q_tail;
	if (qp->q_tail != NULL)
		qp->q_tail->j_next = jp;
	else
		qp->q_head = jp;	/* list was empty */
	qp->q_tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 */
void
job_remove(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head) {
		qp->q_head = jp->j_next;
		if (qp->q_tail == jp)
			qp->q_tail = NULL;
		else
			jp->j_next->j_prev = jp->j_prev;
	} else if (jp == qp->q_tail) {
		qp->q_tail = jp->j_prev;
		jp->j_prev->j_next = jp->j_next;
	} else {
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
	struct job *jp;

	if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
		return(NULL);

	for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
		if (pthread_equal(jp->j_id, id))
			break;

	pthread_rwlock_unlock(&qp->q_lock);
	return(jp);
}
