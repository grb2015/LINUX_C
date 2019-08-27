/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   file:			opmsg.c
*   breif:			设置消息队列的属性.
*   history：		2017-06-03renbin.guo created
*   note：		
*   				//设置消息队列属性
*   				 int msgctl(int msqid, int cmd, struct msqid_ds *buf);
*   				 对消息队列msgid执行cmd操作,系统定义了三种cmd操作:
*   				 IPC_STAT	:	获取消息队列对应的msqid_ds数据结构
*   				 				并将其保存到buf指向的地址空间。
*   				 IPC_SET	:	用来设置消息队列的属性,要设置的属性保存在buf中
*   				 IPC_RMID	:	从内核中删除msgid消息队列
*
*
*   usage：		
*   				[root@grb_host 5_MsgQueue]# ./opmsg 
*   				1------------------------------
*   				****information of message queue 98304****
*   				current number of bytes on queue is 0
*   				number of messages in queue is 0
*   				max number of bytes on queue is 65536
*   				pid of last msgsnd is 0
*   				pid of last msgrcv is 0
*   				msg uid is 0
*   				msg gid is 0
*   				******information end!**********
*   				1------------------------------
*
*
*   				2--------send a message to queue ----------------------
*   				****information of message queue 98304****
*   				current number of bytes on queue is 260
*   				number of messages in queue is 1
*   				max number of bytes on queue is 65536
*   				pid of last msgsnd is 4174
*   				pid of last msgrcv is 0
*   				msg uid is 0
*   				msg gid is 0
*   				******information end!**********
*   				2------------------------------
*
*
*   				3-----------msgctl-------------------
*   				****information of message queue 98304****
*   				current number of bytes on queue is 260
*   				number of messages in queue is 1
*   				max number of bytes on queue is -1534974752
*   				pid of last msgsnd is 4174
*   				pid of last msgrcv is 0
*   				msg uid is 3
*   				msg gid is 2
*   				******information end!**********
*   				3-----------------------------
*
*
*   				4-----------delete msg queue-------------------
*   				msgctl failed
*
*   				[root@grb_host 5_MsgQueue]# 
*   				
*
================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define     BUF_SIZE	256
#define     PATH_NAME 	"."
#define     PROJ_ID		32


/**
 * @brief 	打印字符串s,然后调用exit(1)退出本进程
 *
 * @param s	:要打印的内容
 * 
 * @returns	: NULL
 */
void error_handler(char *s)
{
		printf("%s\n",s);
		exit(1);
}

void getmsgattr(int msgid,struct msqid_ds msg_info)
{
		if( msgctl(msgid,IPC_STAT,&msg_info) )
				error_handler("msgctl failed\n");
			
		printf("****information of message queue %d****\n",msgid);
		/* ctime这几个不注释掉的话会出错　 core dump */
	//	printf("last msgsnd to msq time is %s\n", ctime(&(msg_info.msg_stime)));
	//	printf("last msgrcv time from msg is %s\n", ctime(&(msg_info.msg_rtime)));
	//	printf("last change msq time is %s\n", ctime(&(msg_info.msg_ctime)));
		printf("current number of bytes on queue is %d\n",msg_info.msg_cbytes);
		printf("number of messages in queue is %d\n",msg_info.msg_qnum);
		printf("max number of bytes on queue is %d\n",msg_info.msg_qbytes);
		printf("pid of last msgsnd is %d\n",msg_info.msg_lspid);
		printf("pid of last msgrcv is %d\n",msg_info.msg_lrpid);

		printf("msg uid is %d\n",msg_info.msg_perm.uid);
		printf("msg gid is %d\n",msg_info.msg_perm.gid);
		printf("******information end!**********\n",msgid);	
}
int main(int argc, char *argv[])
{
		/* 用户自定义的消息缓冲区 */	
		struct mymsgbuf
		{
				long msgtype;
				char ctrlstring[BUF_SIZE];
		}msgbuffer;

		int		qid;		/* 消息队列标志 */
		int 	msglen;		/* 消息长度*/
		int 	msgkey;		/* 消息队列的键值 */

		struct	msqid_ds	msg_attr;	/* 消息队列属性的对象 */

		/* 获得键值 */
		msgkey = ftok(PATH_NAME,PROJ_ID);
		if ( msgkey == -1)
			error_handler("ftok failed\n");	

		/* 创建消息队列(如不存在),存在则返回qid */
		qid = msgget(msgkey,IPC_CREAT|0660);
		if(qid == -1)
				error_handler("msgget failed\n");

		/* 1. 获取消息队列的属性.  保存msg_attr,并输出 */
		printf("1------------------------------\n");
		getmsgattr(qid,msg_attr);
		printf("1------------------------------\n\n\n");

		/* 2. 发送一条消息到消息队列 ,再获取属性*/
		msgbuffer.msgtype = 2;
		strcpy(msgbuffer.ctrlstring,"Another messge");
		msglen = sizeof(struct mymsgbuf) -4;	// 减4是减去msgtype的长度 ?	
		if (msgsnd(qid,&msgbuffer,msglen,0))
				error_handler("msgsend failed\n");
		printf("2--------send a message to queue ----------------------\n");
		getmsgattr(qid,msg_attr);
		printf("2------------------------------\n\n\n");

		/* 3.设置消息队列的属性,再获取属性 */
		msg_attr.msg_perm.uid = 3;
		msg_attr.msg_perm.gid = 2;
		if(msgctl(qid,IPC_SET,&msg_attr) )
				error_handler("msgctl set failed\n");
		printf("3-----------msgctl-------------------\n");
		getmsgattr(qid,msg_attr);
		printf("3-----------------------------\n\n\n");

		/* 4.删除消息队列 */
		if (msgctl(qid,IPC_RMID,NULL))
		{
				error_handler("msgctl delete failed");
		}
		printf("4-----------delete msg queue-------------------\n");
		getmsgattr(qid,msg_attr);
		printf("4-----------delete msg queue-------------------\n");
}
