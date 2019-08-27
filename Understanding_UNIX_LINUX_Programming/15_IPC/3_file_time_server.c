/* file_ts.c - read the current date/time from a file 
 *     usage: file_ts filename
 *    action: writes the current time/date to filename
 *     note: uses fcntl()-based locking
 */

 /*  brief : 进程间通信可以共享内存，命名管道，还有通过文件。
             这里就是通过文件来通信。实现前面的日期时间服务器
             文件通信也会存在竞态问题，所以服务器写的时候要加
            写所，同样，客户端读的时候要加读锁。
     history: 2017-05-15 renbin.guo  created

     note  : ./3_server  filename        // 时间日期数据通过filename 来共享 
   *
  */

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>

#define  oops(m,x)  { perror(m); exit(x); }

main(int ac, char *av[])
{
	int	fd;
	time_t  now;
	char    *message;

	if ( ac != 2 ){
		fprintf(stderr,"usage: file_ts filename\n");
		exit(1);
	}
	if ( (fd = open(av[1],O_CREAT|O_TRUNC|O_WRONLY,0644)) == -1 )
		oops(av[1],2);

    // 隔一秒更新一次时间
	while(1)
	{
		time(&now);
		message = ctime(&now);		/* compute time	     */

		lock_operation(fd, F_WRLCK);	/* lock for writing  */

		if ( lseek(fd, 0L, SEEK_SET) == -1 )
			oops("lseek",3);

		// 将时间日期写到文件中
		if ( write(fd, message, strlen(message)) == -1 )
			oops("write", 4);

		lock_operation(fd, F_UNLCK);	/* unlock file	     */
		sleep(1);			/* wait for new time */
	}
}

lock_operation(int fd, int op)
{
	struct flock lock;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len = 0;
	lock.l_pid = getpid();
	lock.l_type = op;

	if ( fcntl(fd, F_SETLKW, &lock) == -1 )
		oops("lock operation", 6);
}
