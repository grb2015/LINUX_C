/* twordcount1.c - threaded word counter for two files. Version 1 */

/*************************************************
* 	brief : 线程共享全局变量(资源的问题)
*		该程序开两个线程,统计file1和file2中的单词数,求总的单词数
*	
*	note  :  total_words++不是一个原子操作,它从内存中取出total_words到寄存器
*	         执行加1，然后再拷贝会寄存器.
*		 所以有可能当一个线程还在寄存器中计算时，另外一个线程从内存中读取*		  这样就读到的就是旧值了
*		
*		usage: 	
			./3_1 file1 file2

grb-sim@xxx:~/github/LINUX_CMD/14_thread$ ./3_1 file1 file2
        [root@localhost 14_thread]# ./3_twordcount1 file1 file2 
            200: file1      // 说明在打印之前，计算已经完成了。
            200: file2
            200: total words
        [root@localhost 14_thread]# 

        



		结论：好像也没有出错啊，是正常的



*
*
*
*
*************************************************/

#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>

int	  total_words ;

main(int ac, char *av[])
{
	pthread_t t1, t2;		/* two threads */
	void	  *count_words(void *);

	if ( ac != 3 ){
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}
	total_words = 0;
	pthread_create(&t1, NULL, count_words, (void *) av[1]);
	pthread_create(&t2, NULL, count_words, (void *) av[2]);
	pthread_join(t1, NULL);
    printf("%5d: %s\n", total_words, av[1]);
	pthread_join(t2, NULL);
    printf("%5d: %s\n", total_words, av[2]);
	printf("%5d: total words\n", total_words);
}
void *count_words(void *f)
{
	char *filename = (char *) f;
	FILE *fp;
	int  c, prevc = '\0';
	
	if ( (fp = fopen(filename, "r")) != NULL ){
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) )
				total_words++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(filename);
	return NULL;
}

