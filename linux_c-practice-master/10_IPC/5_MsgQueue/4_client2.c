/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			client2.c
*   breif:				
*   history：		2017-06-04renbin.guo created
*   note：		
*   usage：		
*
================================================================*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>


#define BUF_SIZE	256
#define PROJ_ID		32
#define PATH_NAME	"/tmp"
#define SERVER_MSG	1
#define CLIENT_MSG	2

int main(void)
{
		/*用户自定义消息缓冲区*/
		struct mymsgbuf {
				long msgtype;
				char ctrlstring[BUF_SIZE];
		} msgbuffer;
		int	qid; 	/*消息队列标识符*/
		int	msglen;
		key_t	msgkey;

		/*获取键值*/
		if ((msgkey = ftok(PATH_NAME, PROJ_ID)) == -1)
		{
				perror("ftok error!\n");
				exit(1);
		}

		/* 创建(消息队列不存在)/获得消息队列的qid */
		if ((qid = msgget(msgkey, IPC_CREAT|0660)) == -1)
		{
				perror("msgget error!\n");
				exit(1);
		}

		while (1) 
		{
				/* 1. 通过消息队列从服务器获得消息,并打印 */
				printf ("begin to rcv: \n" );  
				if (msgrcv(qid,&msgbuffer,BUF_SIZE,SERVER_MSG,0) == -1)         //      这里也是阻塞的，如果没有可读的消息，则阻塞
				{
						perror ("Server msgrcv error!\n");
						exit (1);
				}
				printf ("server: %s\n", msgbuffer.ctrlstring);  

				/* 2. 从键盘输入聊天信息发送给服务器 */
				printf ("me(client): ");
				fgets ( msgbuffer.ctrlstring, BUF_SIZE, stdin);     // 注意这里是阻塞的，你不输入信息，就不向后执行了。
				if (strncmp ("exit", msgbuffer.ctrlstring, 4) == 0) 
				{
						break;
				}
					/* 注意这里send之前要先填充msgbuffer结构，指定它为CLIENT_MSG */
				msgbuffer.ctrlstring[strlen(msgbuffer. ctrlstring)-1] = '\0';
				msgbuffer.msgtype = CLIENT_MSG;
				if(msgsnd(qid,&msgbuffer,strlen(msgbuffer.ctrlstring) + 1,0) == -1)
				{
						perror("client msgsnd error!\n");
						exit(1);
				}
		}
		exit(0);
}


