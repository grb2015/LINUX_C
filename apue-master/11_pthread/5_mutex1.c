/*================================================================
 *   Copyright (C) 2017 free for learn.
 *   
 *   file:			5_mutex1.c
 *   breif:			互斥量pthread_mutex_t的应用(实际上就是互斥锁)
 *   				
 *   				实现功能：
 *   					当多个线程需要访问动态分配的对象时,我们对该
 *   				对象进行加锁。实现互斥访问。
 *   				顺便加了一个计数，表面当前的访问该对象的线程数。
 *   				疑问：
 *   					既然是互斥访问，那么计数不应该总是1吗?不会啊，每访问一次，计数就加1
 *   history：		2017-06-08renbin.guo created
 *   note：		
 *   usage：		
 *
 ================================================================*/


#include <stdlib.h>
#include <pthread.h>

struct foo {

		int             f_count;
		pthread_mutex_t f_lock;			/* f_lock就是一个互斥量 */
		int             f_id;
		/* ... more stuff here ... */

}

struct foo *	foo_alloc(int id) /* allocate the object */
{

		struct foo *fp;

		if ((fp = malloc(sizeof(struct foo))) != NULL) {

				fp->f_count = 1;
				fp->f_id = id;
				/* 初始化互斥量f_lock */
				if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {

						free(fp);
						return(NULL);

				}
				/* ... continue initialization ... */

		}
		return(fp);

}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{

		/* 对互斥量f_lock加锁 */
		pthread_mutex_lock(&fp->f_lock);
		fp->f_count++;
		/* 对互斥量f_lock解锁 */
		pthread_mutex_unlock(&fp->f_lock);

}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{

		pthread_mutex_lock(&fp->f_lock);
		if (--fp->f_count == 0) {
				/* last reference */
				pthread_mutex_unlock(&fp->f_lock);
				pthread_mutex_destroy(&fp->f_lock);
				free(fp);

		} else {

				pthread_mutex_unlock(&fp->f_lock);

		}

}

