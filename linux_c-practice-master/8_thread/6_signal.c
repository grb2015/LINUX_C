/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file：6_signal.c
*   breif：
*   	线程同步的第三种方式，信号(或者叫异步信号，因为线程与任何信号都是异步的)
*   history：renbin.guo created2017-05-25
*   note：
*   usage：
*
================================================================*/
#include  <pthread.h>

/* 向特定的线程发送信号signo */
int pthread_kill(pthread_t threadid,int signo);

/* 函数pthread sigmask 用来设置线程的信号屏蔽码 */
int pthread_sigmask(int how,const sigset_t *newmask,sigset_t *oldmask);

/* 函数sigwait用来阻塞线程，等待set中指定的信号之一到达，并将到达的信号存入*sig中 */
int sigwait(const sigset_t *set,int *sig);
