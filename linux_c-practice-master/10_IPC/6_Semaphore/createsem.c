/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			createsem.c
*   breif:			创建信号集并对信号集中所有信号进行初始化
*   history：		2017-06-04renbin.guo created
*   note：		
*   				
*   usage：		
*
================================================================*/


int createsem(const char *pathnaem,int proj_id,int members,int init_val)
{
		key_t	msgkey;
		int		index;
		int 	sid;
		union 	semun 	semopts;	//系统定义的联合体

		/* 1. 跟消息队列一样，先获得msgkey */
			/* 约定，大写的函数代表封装了错误处理 */
		msgkey = FTOK(pathname,proj_id);

		/* 2. 创建信号量 */

		
		/**
		 * @briefa					:创建信号集 
		 *
		 * @param msgkey			:由ftok得到的key
		 * @param members			:信号集包含的信号个数
		 * @param IPC_CREATE|0666	:标志
		 *
		 * @return 					:创建成功返回一个信号集的标志符号
		 * 							 失败 -1	
		 */
		sid = semget(msgkey,members,IPC_CREATE|0666);
		if( sid == -1)
				error_handler("semget error!\n");

		/* 3.设置信号量属性 */
		semopts.val = init_val;	// init_val是传进来的参数
		for(index = 0;index<members ; index++)
				semctl(sid,index,SETVAL,semopts);

		return (sid);
}

