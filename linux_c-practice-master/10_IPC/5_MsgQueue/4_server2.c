/**
 * @file server2.c
 * @brief 	使用消息队列实现一台主机的两个进程间"聊天"
 * @author renbin.guo
 * @version 1
 * @date 2017-06-04
 *
 * note:
 *
 * usage:
 * 					[root@grb_host 5_MsgQueue]# ./server2 
 * 					me(server):hi,client
 * 					client: hi,server
 * 					me(server):exit
 * 					inputed exit
 * 					[root@grb_host 5_MsgQueue]# 
 *
 * 					
 * 					[root@grb_host 5_MsgQueue]# ./client2 
 * 					begin to rcv: 
 * 					server: hi,client
 * 					me(client): hi,server
 * 					begin to rcv: 
 * 					Server msgrcv error!
 * 					: Identifier removed
 * 					[root@grb_host 5_MsgQueue]#
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define BUF_SIZE	256
#define PROJ_ID		32
#define PATH_NAME	"/tmp"
#define SERVER_MSG	1
#define CLIENT_MSG	2

void error_handler(char *s)
{
	printf("%s\n",s );
	exit(1);
}
int main(int argc, char const *argv[])
{
	struct mymsgbuf
	{
		long	msgtype;
		char 	ctrlstring[BUF_SIZE];
	}msgbuffer;

	int 	qid;
	int 	msglen;
	key_t	msgkey;


	/* 创建消息队列 */
	msgkey = ftok(PATH_NAME,PROJ_ID);
	if (msgkey == -1)
	{
			error_handler("ftok error\n");	
	}

	qid = msgget(msgkey,IPC_CREAT|0660);
	if (qid == -1)
	{
		error_handler("mssget error\n");
	}


	/* 通过消息队列在不同进程间通信，实现"聊天" */
	/* 不断进行这样的循环,直到输入exit */
	/* 循环 */
	while(1)
	{
		printf("me(server):");
		/* 1.从键盘输入聊天信息,发送给客户端 */
		fgets(msgbuffer.ctrlstring,BUF_SIZE,stdin);
				/* 如果本端输入了exit则删除消息队列，并退出进程 */

		if(!strncmp("exit",msgbuffer.ctrlstring,4))		//这个地方尽量还是写成 == 0 ,可读性好点
		{
			printf("inputed exit\n");
			msgctl(qid,IPC_RMID,NULL);
			break;
		}
				/* 如果输入的不是exit,则将输入的内容发送给消息队列 */
		msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring)-1] = '\0';
		msgbuffer.msgtype = SERVER_MSG;
							// +1是添加了一个'\0'
		if(msgsnd(qid,&msgbuffer,strlen(msgbuffer.ctrlstring)+1,0))			// 这个地方还是写成== -1好点
				error_handler("msgsnd error!\n");
		
		//printf("msgsended!\n");	//for debug
		
		/* 2.接着从读取并打印客户端的消息,CLIENT_MSG做参数 */         //      这里也是阻塞的，如果没有可读的消息，则阻塞
		if( msgrcv(qid,&msgbuffer,BUF_SIZE,CLIENT_MSG,0) == -1)			//这里必须写成 == -1 ,因为成功是返回读到的字节数
				error_handler("msgrcv error!\n");
		printf("client: %s\n",msgbuffer.ctrlstring);
	}
	exit(0);
}
