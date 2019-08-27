 /*================================================================
	2 *   Copyright (C) 2017 free for learn.
	3 *   
	4 *   file:           5_mutex2.c
	5 *   breif:          通过让所有线程以相同的顺序加锁来避免死锁。  
	6 *                   
	7 *                   
	8 *                               
	9 *   history：       2017-06-08renbin.guo created
	10 *  note：          
							bug:
								疑惑1:	不是很明白维护的fh是一个怎样的结构
								疑惑2:  	
									 	疑问2，这里指针与指针作了==运算 
										if (tfp == fp) {
	
	11 *                   
	12 *   usage：     
	13 *
 ================================================================*/

#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) ( ((unsigned long)id) % NHASH )		/* HASH其实只是 id%29 对29取余而已*/

struct foo *fh[NHASH];			/* 指针数组，NHASH个strct foo*类型的指针  */

/* 分配并初始化一个互斥量hashlock */
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;


struct foo {							/* 链表节点的数据结构,因为成员中有指向本类型结构的指针  */
		int             f_count;
		pthread_mutex_t f_lock;
		int             f_id;
		struct foo     *f_next; /* protected by hashlock */
		/* ... more stuff here ... */
};




struct foo *
foo_alloc(int id) /* allocate the object */
{
		struct foo	*fp;
		int			idx;

		if ((fp = malloc(sizeof(struct foo))) != NULL) {
				fp->f_count = 1;
				fp->f_id = id;

				/* 初始化结构foo中的互斥量f_lock */
				if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
						free(fp);
						return(NULL);
				}
				idx = HASH(id);

				/* 1.尝试获取hashlock互斥量，若不能获取,可能阻塞 */
				pthread_mutex_lock(&hashlock);

					/* 获得了互斥量*/
				fp->f_next = fh[idx];		/* 疑问1.这样是在做什么 ? 岂不是fp->f_next = fp了，指向它本身了 */
				fh[idx] = fp;				/* 将刚刚动态分配的对象加入到fh[ind]数组(列表)中 */

				/* 2.尝试获取f_lock互斥量，若不能获取,可能阻塞 */
				pthread_mutex_lock(&fp->f_lock);
													/* 执行到这里，两个互斥量都获得了 */

				/* 3.释放两个锁 */
				pthread_mutex_unlock(&hashlock);
				/* ... continue initialization ... */
				pthread_mutex_unlock(&fp->f_lock);
		}
		return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
		pthread_mutex_lock(&fp->f_lock);
		fp->f_count++;
		pthread_mutex_unlock(&fp->f_lock);
}

struct foo *
foo_find(int id) /* find an existing object */
{
		struct foo	*fp;

		/* 显然，在访问fp之前,需要顺序获得hashlock互斥量和fp->f_lock互斥量,从而避免死锁 */
		pthread_mutex_lock(&hashlock);
		for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
				if (fp->f_id == id) {
						foo_hold(fp);		/* foo_hold里面有 pthread_mutex_lock(&fp->f_lock)*/
						break;
				}
		}
		pthread_mutex_unlock(&hashlock);
		return(fp);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
		struct foo	*tfp;
		int			idx;

		pthread_mutex_lock(&fp->f_lock);
		if (fp->f_count == 1) { /* last reference */
				pthread_mutex_unlock(&fp->f_lock);
													/* 在unlock和lock之间，可能这个节点又被别人访问了，所以下面要再次检查count */
				pthread_mutex_lock(&hashlock);
				pthread_mutex_lock(&fp->f_lock);
				/* need to recheck the condition */
				/* 这里必须重新检查count */
				if (fp->f_count != 1) 
				{
						fp->f_count--;
						pthread_mutex_unlock(&fp->f_lock);
						pthread_mutex_unlock(&hashlock);
						return;
				}
				/* remove from list */
				idx = HASH(fp->f_id);
				tfp = fh[idx];
				/* 疑问2，这里指针与指针作了==运算 */
				if (tfp == fp) {
						fh[idx] = fp->f_next;
				} else {
						while (tfp->f_next != fp)
								tfp = tfp->f_next;
						tfp->f_next = fp->f_next;
				}
				pthread_mutex_unlock(&hashlock);
				pthread_mutex_unlock(&fp->f_lock);
				pthread_mutex_destroy(&fp->f_lock);
				free(fp);
		} else {
				fp->f_count--;
				pthread_mutex_unlock(&fp->f_lock);
		}
}
