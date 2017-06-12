/***********************************************************
2 *   brief : 
3             version2 中使用锁解决了多个线程对进程中的同一全局变量的正确访问，但是效率比较低
4             这里为每一个线程开辟自己的(全局)变量，属于自己的变量，其它进程虽然可以访问，但是不要访问，分别计算各个文件中的单词数目，最后再加在一起。
    history : 2017-05-11 renbin.guo created


	note:
			 这种方法的扩展性不是很好,如果要同时计算N各文件呢?

5 *
6 ***********************************************************/

/* twordcount3.c - threaded word counter for two files.	
 *		 - Version 3: one counter per file
 */

#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>

struct arg_set {		/* two values in one arg */
		char *fname;	/* file to examine	 */
		int  count;	/* number of words	 */
};

main(int ac, char *av[])
{
	pthread_t      t1, t2;		/* two threads */
	/* 实际上,args1 和args2是全局的，所有线程都可访问 */
	struct arg_set args1, args2;	/* two argsets */
	void	       *count_words(void *);

	if ( ac != 3 ){
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}
	args1.fname = av[1];
	args1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *) &args1);

	args2.fname = av[2];
	args2.count = 0;
	pthread_create(&t2, NULL, count_words, (void *) &args2);

	/* 必须调用join,确保t1结束了，此时得到的count才能保证是正确的。 */
	pthread_join(t1, NULL);
	//pthread_join(t2, NULL);		//renbin.guo modified 2017-06-13  线程1的计算结果不需要等线程2结束了才打印啊.
									//的确是，但至少，线程2的计算结果必须等线程1退出了才能打印
									//显然，如果我们计算多个文件，那么最后调用pthread_join的函数，必须等待前面所有的
									//线程计算完了之后才能打印。
	printf("%5d: %s\n", args1.count, av[1]);
	pthread_join(t2, NULL);
	printf("%5d: %s\n", args2.count, av[2]);
	printf("%5d: total words\n", args1.count+args2.count);
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
	return NULL;
}
