/* twordcount4.c - threaded word counter for two files.	
 *		 - Version 4: condition variable allows counter
 *                            functions to report results early 
 */
/************************************************************************************  
    breif :
            线程间通信。
            在计算单词的程序中，如果同时计算两个文件，一个文件很大，一个很小。我们需要哪个小的
            一计算出来之后，就通知调用它的线程它的统计结果，如何实现呢？

            这里通过选举中心的例子,解释了互斥变量，锁,条件变量的使用.还是使用之前的单词统计的程序

   history: 2017-05-11 renbin.guo created
    
        usage: 这里可以多开一些线程，计算n各文件，以便看出效果
                      
************************************************************************************** */


#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>

struct arg_set {		/* two values in one arg*/
		char *fname;	/* file to examine	*/
		int  count;	/* number of words	*/
};

struct arg_set  *mailbox = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  flag = PTHREAD_COND_INITIALIZER;

main(int ac, char *av[])
{
	pthread_t      t1, t2;		/* two threads */
	struct arg_set args1, args2;	/* two argsets */
	void	       *count_words(void *);
	int            reports_in = 0;
	int	       total_words = 0;

	if ( ac != 3 ){
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}
	pthread_mutex_lock(&lock);	/* lock the report box now */

	args1.fname = av[1];
	args1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *) &args1);

	args2.fname = av[2];
	args2.count = 0;
	pthread_create(&t2, NULL, count_words, (void *) &args2);

	while( reports_in < 2 ){
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&flag, &lock); /* wait for notify */  // 这里会阻塞，挂起线程，进行等待。除非线程1或2中调用了pthread_cond_signal(&flag);
		                                                        // 光得到这个flag还不够，后一个参数表示他还要得到锁才可以解除阻塞(不是这样理解)
                                                                // 第二个参数到底是干嘛的还有待分析
		printf("MAIN: Wow! flag was raised, I have the lock\n");
		printf("%7d: %s\n", mailbox->count, mailbox->fname);
		total_words += mailbox->count;
		if ( mailbox == &args1) 
			pthread_join(t1,NULL);
		if ( mailbox == &args2) 
			pthread_join(t2,NULL);
		mailbox = NULL;
		pthread_cond_signal(&flag);	/* announce state change */
		reports_in++;
	}
	printf("%7d: total words\n", total_words);
}
void *count_words(void *a)
{
	struct arg_set *args = a;	/* cast arg back to correct type */
	FILE *fp;
	int  c, prevc = '\0';
	
	if ( (fp = fopen(args->fname, "r")) != NULL ){
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) )
				args->count++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(args->fname);
	printf("COUNT: waiting to get lock\n");
	pthread_mutex_lock(&lock);	/* get the mailbox */       ///子线程尝试获取锁，如果没得到，则阻塞在这里，获取锁了还不够。还有邮箱为空
	                                                        // 才能用。这里的锁，其实保护的是mailbox全局变量，因为上面主线程中有
	                                                        // total_words += mailbox->count;所以mailbox应该是互斥的
	printf("COUNT: have lock, storing data\n");
	if ( mailbox != NULL ){     // 如果当前邮箱不为空，则说明有别的在用，则阻塞，等待flag和lock
		printf("COUNT: oops..mailbox not empty. wait for signal\n");
		pthread_cond_wait(&flag,&lock);  // 这里会阻塞，挂起线程，进行等待
	}
	mailbox = args;			/* put ptr to our args there */
	printf("COUNT: raising flag\n");
	pthread_cond_signal(&flag);	/* raise the flag  */
	printf("COUNT: unlocking box\n");
	pthread_mutex_unlock(&lock);	/* release the mailbox */
	return NULL;
}
