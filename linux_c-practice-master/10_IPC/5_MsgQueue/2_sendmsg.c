/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			sendmsg.c
*   breif:			msgsend()向消息队列写数据
*   history：		2017-06-03renbin.guo created
*   note：		
*   usage：		
*   				[root@grb_host 5_MsgQueue]# ./sendmsg 
*   				created msgkey successful ! msgkey = 537025462
*   				create msgqueue  successful ! qid = 0
*   				[root@grb_host 5_MsgQueue]# ipcs
*					------ Message Queues --------
*					key        msqid      owner      perms      used-bytes   messages    
*					0x20025bb6 0          root       666        780          3   
*
*
================================================================*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define     BUF_SIZE	256
#define     PATH_NAME	"."
#define     PROJ_ID		32

int main(int argc, char *argv[])
{
		/* 用用户自定义消息缓冲 */
		struct mymsgbuf
		{
				long msgtype;
				char ctrlstring[BUF_SIZE];
		}msgbuffer;
		int		qid;
		int		msglen;
		key_t	msgkey;

		/* 获取键值 */
		if( (msgkey = ftok(PATH_NAME,PROJ_ID))  == -1) 	
		{
				perror("msgger error!\n");
				exit(1);
		}
		else	
		{
				printf("created msgkey successful ! msgkey = %d\n",msgkey);
		}
		/* 用得到的键值创建消息队列,返回消息队列id */	
		if ( (qid = msgget(msgkey,IPC_CREAT|0666)) == -1)
		{
				perror("msgget error!\n");
				exit(1);
		}
		else	
		{
				printf("create msgqueue  successful ! qid = %d\n",qid);
		}
		/* 填充消息结构，发送到消息队列 */
		msgbuffer.msgtype = 3;
		strcpy(msgbuffer.ctrlstring,"hello,msgqueue");
		msglen = sizeof(struct mymsgbuf) - 4;	/* 为什么要减去4 */
		if( msgsnd(qid,&msgbuffer,msglen,0) == -1)
		{
			perror("msgget error!\n");
			exit(1);
		}
		exit(0);
}
