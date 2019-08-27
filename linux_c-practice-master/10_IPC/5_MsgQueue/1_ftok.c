/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			ftok.c
*   breif:			每个消息队列对一一个键值,ftok函数得到一个键值	
*   history：		2017-06-03renbin.guo created
*   note：		
*   				ftok返回的键值可以提供给函数msgget(),msgget()根据这个
*   				键值创建一个新的消息队列或者访问一个已存在的消息队列。
*   usage：		
*   		[root@grb_host 5_MsgQueue]# ./ftok 
*   		16931766l
*   		33708982l
*   		50486198l
*   		67263414l
*   		84040630l
*   		[root@grb_host 5_MsgQueue]# 
*
================================================================*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
int main(int argc, char *argv[])
{
		int		i;
		for (i = 1; i <= 5; i++)
				/*key_t ftok(const char *pathname, int proj_id);*/
				/* 根据pathname和proj_id生成唯一的键值 */
				printf("%ul\n",ftok(".",i));
		exit(0);
}

