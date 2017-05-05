/****************************************************************************
 * lsrv1.c  
 * License server server program version 1
 */
 
/*  brief : 许可证服务器程序服务器端。
            licence 管理功能: 没有没有很多人在使用该程序，服务器发送票据给
*           客户端进程，给予许可，如果达到了最大的程序实例数，服务器发送无可用
*           票据的消息给客户，并且通知客户稍后再试或者购买支持功能更多用户版本的
*           软件
*   history : 2017-05-05 renbin.guo created
*
*/          


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>
#define MSGLEN		128		/* Size of our datagrams */

int main(int ac, char *av[])
{
	struct sockaddr_in client_addr;
	socklen_t addrlen;
	char      buf[MSGLEN];
	int	  ret;
	int       sock;

	sock = setup();

	while(1) {
		addrlen = sizeof(client_addr);
		ret = recvfrom(sock,buf,MSGLEN,0,
				(struct sockaddr*)&client_addr,&addrlen);
		if ( ret != -1 ){
			buf[ret] = '\0';
			narrate("GOT:", buf, &client_addr);
			handle_request(buf,&client_addr,addrlen);
		}
		else if ( errno != EINTR )
			perror("recvfrom");
	}
}

