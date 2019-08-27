/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			semval_op.c
*   breif:			获取和设置单个信号量的值 ---semctl()	
*   history：		2017-06-04renbin.guo created
*   note：		
*   usage：		
*
================================================================*/

int semval_op(int semid, int index, int cmd)
{
		if(index < 0)
				error_handler("index invaid\n");

		/* 获取和设置单个信号 */
		if(cmd == GETVAL || cmd == SETVAL)

				/* 		int semctl(int semid, int semnum, int cmd, ...);	
				 * 		可见参数是可变的,依赖于第三个参数cmd
				 *
				 *
				 * */

				/* semid和index不是一样的么 */				
				return semctl(semid,index,cmd,0);

		printf("cannot surport cmd :%d\n",cmd);
		return -1;
}

