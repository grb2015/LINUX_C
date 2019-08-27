/**
 * @file condvar.c
 * @brief 		使用条件变量来保护链表workq,实现对其的互斥访问.
 * 				其实通过互斥锁就可以对链表进行互斥访问了，条件变量的主要作用是防止
 * 				CPU轮询等待条件.cond_wait后，就进入睡眠状态，等条件满足后，被唤醒.
 *
 * @author		renbin.guo 
 * @version 	1
 * @date 2017-06-10
 *
 * note:
 * 				条件变量的部分参见 <<linux-programming-interface >> 30_thread讲的比较清晰
 *				cond_wait会执行下面的步骤:
 *					1.解锁互斥量
 *					2.阻塞调用线程，直到被唤醒 
 *					3.重新加锁
 * 		
 */
#include <pthread.h>

/* 链表的节点 */
struct msg {
	struct msg *m_next;
	/* ... more stuff here ... */
};

struct msg *workq;

/* 初始化条件变量qready */
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;


/* 从链表workq中拿出头节点来处理 */
void
process_msg(void)
{
	struct msg *mp;

	for (;;) {
		pthread_mutex_lock(&qlock);
		while (workq == NULL)
				/*  */
			pthread_cond_wait(&qready, &qlock);
		mp = workq;
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		/* now process the message mp */
	}
}


/* 添加节点 ,加入到链表头*/
void
enqueue_msg(struct msg *mp)
{
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);	//激活一个等待添加变量成立的线程来执行
}
