/**
 * @file 	1_client3.c
 * @brief	演示信号量的用法。客户端循环的检查信号量的值，
 * 			如果大于0就打印一条语句。小于0则退出 
 * @author 	renbin.guo
 * @version 1
 * @date 	2017-06-04
 *
 * usage:
 * 			[root@grb_host 6_Semaphore]# ./client3 
 * 			Still 3 resources can be used
 * 			Still 2 resources can be used
 * 			Still 1 resources can be used
 * 			No more resources can be used!
 * 			[root@grb_host 6_Semaphore]#
 */

#include <sys/types.h>
#include <linux/sem.h>

int main(void)
{
	key_t		key;
	int		semid, semval;
	union semun	semopts;

	if((key = ftok (".",'s')) == -1)
	{
		perror ("ftok error!\n");
		exit (1);
	}

	if((semid = semget (key, 1, IPC_CREAT | 0666)) == -1)
	{
		perror ("semget error!\n");
		exit (1);
	}

	while(1)
	{
			/* 获取信号量的值 */
		if ((semval = semctl(semid, 0, GETVAL, 0)) == -1)
		{
			perror ("semctl error!\n");
			exit (1);
		}
		if (semval > 0)
		{
			printf ("Still %d resources can be used\n", semval);
		}
		else
		{
			printf ("No more resources can be used!\n");
			break;
		}

		sleep (3);
	}
	
	exit (0);
}
