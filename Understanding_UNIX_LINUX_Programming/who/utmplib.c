/*
 * 	name	: 	utmplib.c
 * 	author	:	renbin.guo
 * 	brief	:	the APIs of buffered verion 'who'
 * 	note	:	can used as static lib (可作为静态库使用)
 * 	history	:	
 * 			2017-03-27 : renbin.guo created 
 *
 *
 * */

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<utmp.h>

#define	NRECS 	16	//每次缓冲16个记录
#define NULLUT	((struct utmp*)NULL)
#define UTSIZE	(sizeof(struct utmp))

static	char utmpbuf[NRECS*UTSIZE];	//开辟一个内存数组作为buf，来存16个记录
static 	int num_recs;			/* num_recs 实际读到的记录数 */
static 	int cur_rec;			/* 当前的记录 */
static	int fd_utmp = -1;		/*显然，这里fd作为全局变量，后面的函数中都会用到它*/

int utmp_open(char * filename)
{
	fd_utmp = open(filename,O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}
struct  utmp* utmp_next()
{
	struct utmp* recp;
	if(fd_utmp == -1)
		return NULLUT;
	/*utmp_reload 一次从文件中read 16个记录*/
	if(cur_rec == num_recs && utmp_reload() == 0)
		return NULLUT;
	recp =(struct utmp*) &utmpbuf[cur_rec*UTSIZE]; 
	cur_rec++;	//注意这里增1
	return recp;
}
/*****************************************************
 * 	read next bunch of records into buffer 
 * 	一次读取16个记录
 * 
 *****************************************************/
int utmp_reload()
{
	int	amt_read;
	amt_read = read(fd_utmp,utmpbuf,NRECS*UTSIZE);
	num_recs = amt_read/UTSIZE;
	cur_rec = 0;
	return num_recs;
}
void utmp_close()
{
	if(fd_utmp != -1)
		close(fd_utmp);
}

	

