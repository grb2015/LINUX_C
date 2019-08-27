/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2015.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 30 */

/* prod_no_condvar.c

   A simple POSIX threads producer-consumer example that doesn't use
   a condition variable.

   See also prod_condvar.c.
*/
/**
 * @file prod_no_condvar.c
 * @brief 		生产者消费者问题,开若干个线程作为生产者，主线程作为消费者
 * 			
 * @author 
 * @version 
 * @date 2017-06-12
 *
 * note:
 * 		虽然用互斥锁能解决互斥,程序完全正确。但是主线程轮询等待，浪费CPU
 *
 * usage:
 * 		[root@grb_host linux-programming-interface]# ./prod_no_condvar  5
 * 		T=1: numConsumed=1
 * 		T=2: numConsumed=2
 * 		T=3: numConsumed=3
 * 		T=4: numConsumed=4
 * 		T=5: numConsumed=5
 * 		[root@grb_host linux-programming-interface]# 
 *
 */

#include <stdio.h>
#include <time.h>
#include <pthread.h>
//#include "tlpi_hdr.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static int avail = 0;

/* 生产者 */

static void *
threadFunc(void *arg)
{
    int cnt = atoi((char *) arg);
    int s, j;

    for (j = 0; j < cnt; j++) {
        sleep(1);

        /* Code to produce a unit omitted */

        s = pthread_mutex_lock(&mtx);
        if (s != 0)
			printf("lock error!\n");
            //errExitEN(s, "pthread_mutex_lock");

        avail++;        /* Let consumer know another unit is available */

        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
			printf("unlock error!\n");
            //errExitEN(s, "pthread_mutex_unlock");
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t tid;
    int s, j;
    int totRequired;            /* Total number of units that all
                                   threads will produce */
    int numConsumed;            /* Total units so far consumed */
    int done;
    time_t t;

    t = time(NULL);


    /* Create all threads */
	/* 创建多个线程,让他们去生产东西 */

    totRequired = 0;
    for (j = 1; j < argc; j++) {
        totRequired += atoi(argv[j]);

        s = pthread_create(&tid, NULL, threadFunc, argv[j]);
        if (s != 0)

			printf("lock error!\n");
            //errExitEN(s, "pthread_create");
    }

    /* Use a polling loop to check for available units */

    numConsumed = 0;
    done = 0;

    for (;;) {
        s = pthread_mutex_lock(&mtx);
        if (s != 0)
			printf("lock error!\n");
           // errExitEN(s, "pthread_mutex_lock");

        while (avail > 0) {             /* Consume all available units */

            /* Do something with produced unit */

            numConsumed ++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t),
                    numConsumed);

            done = numConsumed >= totRequired;
        }

        s = pthread_mutex_unlock(&mtx);
        if (s != 0)
				printf("unlock error!\n");
            //errExitEN(s, "pthread_mutex_unlock");

        if (done)
            break;

        /* Perhaps do other work here that does not require mutex lock */

    }

    exit(0);
}
