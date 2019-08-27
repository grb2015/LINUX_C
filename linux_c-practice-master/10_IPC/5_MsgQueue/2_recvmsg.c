/*================================================================
*   Copyright (C) 2017 free for learn.  *   
*   file:			recvmsg.c
*   breif:			从消息队列中读取消息
*   history：		2017-06-03renbin.guo created
*   note：		
*   			 ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
*   			                       int msgflg);
*
*   usage：		
*   			[root@grb_host 5_MsgQueue]# ./sendmsg 
*   			created msgkey successful ! msgkey = 537025462
*   			create msgqueue  successful ! qid = 0
*   			[root@grb_host 5_MsgQueue]# ./recvmsg 
*   			get message hello,msgqueue
*   			[root@grb_host 5_MsgQueue]# 
*
*   			
*
================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define     PATH_NAME	"."
#define     PROJ_ID		32
#define     BUF_SIZE	256
int main(int argc, char *argv[])
{
		struct	mymsgbuf
		{
				long msgtype;
				char ctrlstring[BUF_SIZE];
		}msgbuffer;

		int 	qid;
		int 	msglen;
		key_t	msg_key;

		/* 获取键值 */
		msg_key = ftok(PATH_NAME,PROJ_ID); 
		if(msg_key == -1)
		{
				perror("ftok failed\n");
				exit(1);
		}

		/* 获取qid */
		qid = msgget(msg_key,IPC_CREAT|0660);
		if( qid == -1)
		{
				perror("msgget failed\n");
				exit(1);
		}
		
		/* 读取消息队列 */
		msglen = sizeof(struct mymsgbuf)-4;
		/* 3为msgtype 0 为标志位 */
		if (!msgrcv(qid,&msgbuffer,msglen,3,0) )
		{
				perror("msgrcv failed\n");
				exit(1);
		}
		printf("get message %s\n",msgbuffer.ctrlstring);
		exit(0);
}
		







