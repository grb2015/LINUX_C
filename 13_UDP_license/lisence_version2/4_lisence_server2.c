/****************************************************************************
 * lsrv2.c  
 * License server server program version 2 - features ticket recycling
 */

/***********************************************
 *  brief ：许可证服务器version2        
 *          增加的功能：
 *              1. 处理客户端崩溃
 *              2. 处理服务器崩溃
 *          注意,客户端和服务器只能在同一主机上。因为要发kill(pid,0)信号，网络socket是无法发信号的。
 *              
 *  history: 2017-05-05 renbin.guo created
 *
 *  note:
 *              1. 客户端崩溃
 *              2. 处理服务器崩溃
 *              
 *  history: 2017-05-05 renbin.guo created
 *
 *  note:
 *              1. 客户端崩溃
 *                 导致的后果：
 *                     客户将不会返还票据
 *                     导致服务器列表里有一些ticket一直被占用
 *              2. 处理服务器崩溃
 *              
 *  history: 2017-05-05 renbin.guo created
 *
 *  note:
 *              1. 客户端崩溃
 *                 导致的后果：
 *                     客户将不会返还票据
 *                     导致服务器列表里有一些ticket一直被占用
 *                  解决方法：
 *                      服务器定期检查票据数组,确认其中的每个进程
 *                      是否还活着，如果不存在了，就回收ticket
 *                      (这里有两个独立的操作，等待客户的请求，周期性
 *                      的收回丢失的票据，要注意他们的互相影响,如果当前正在处理请求，则应该关掉收回票据的操作)
 *              2. 服务器崩溃
 *                  后果：
 *                      签出的列表丢失,同时新客户的请求也不会得到响应
 *                  解决:
 *                      1.重启服务器,但是显然,重启后原来的签出列表仍然是丢失，只不过可以提供新的连接。          
 *                      另外,旧的持有票据的客户在归还ticket时，由于签出列表中没有记录中，将被认为是伪造
 *                      的票据.
 *                      2.为了解决签出表丢失的问题，可以进行票据验证。(这个功能在程序中也没有体现啊?) *                      即然后每个客户周期性(从程序中并没有看出是周期性的啊，而是只发了一次?)的向服务器发送票据的副本。这样，重启一段
 *                      时间后,签出表就找回来了。
 *                         需要注意的是:在签出表重建好之前，有新用户请求怎么办?如果给它一个当前没有使用的票据，也许正好
 *                         这个票据正在恢复中(是不是可以再重建好之前,拒绝所有的新连接?)
 ***********************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>

#define	MSGLEN		128

#define RECLAIM_INTERVAL 5		/* reclaim every 60 seconds */
int main(int ac, char *av[])
{
	struct sockaddr client_addr;
	socklen_t addrlen;
	char      buf[MSGLEN];
	int	  ret, sock;
	void	  ticket_reclaim();	/* version 2 addition */
	unsigned  time_left;

	sock = setup();
	// 注册信号处理函数
	signal(SIGALRM, ticket_reclaim); /* run ticket reclaimer */
	alarm(RECLAIM_INTERVAL);	 /* after this delay     */

	while(1) {
		addrlen = sizeof(client_addr);
		ret = recvfrom(sock,buf,MSGLEN,0,
				(struct sockaddr *)&client_addr,&addrlen);
		if ( ret != -1 ){
			buf[ret] = '\0';
			narrate("GOT:", buf, &client_addr);
			// 关掉定时器(当有新的消息时)
			time_left = alarm(0);
			handle_request(buf,&client_addr,addrlen);
			// 消息处理完了之后，重新开定时器
			alarm(time_left);
		}
		else if ( errno != EINTR )
			perror("recvfrom");
	}
}
