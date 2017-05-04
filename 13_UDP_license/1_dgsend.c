/*********************************************************************
 * dgsend.c  - datagram sender
 * 	         usage: dgsend hostname portnum "message"
 * 	        action: sends message to hostname:portnum
 */

/*****************************************************
 * brief : 一个UDP的客户端程序,使用它想服务器发送一条信息，然后退出客户端
 *
 * history: 2017-05-04 renbin.guo created
 *
 * usge:
 * 	1.在另一终端启动服务器
 * 		 [root@grb_host 13_UDP_license]# hostname
 * 		 grb_host
 * 		 [root@grb_host 13_UDP_license]# ./dgrecv 12345
 *
 * 		
 * 	2. 运行客户端
 * 	[root@grb_host 13_UDP_license]# hostname
 * 	grb_host
 * 	[root@grb_host 13_UDP_license]# ./dgsend grb_host 12345 aaa
 * 	make addr: Resource temporarily unavailable
 * 	[root@grb_host 13_UDP_license]#
 *
 * 	bug: make addr: Resource temporarily unavailable
 *
*****************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

#define oops(m,x)  { perror(m);exit(x);}
int make_dgram_client_socket();
int make_internet_address(char *,int, struct sockaddr_in *);

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	char	*msg;			/* send this messag		*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/

	if ( ac != 4 ){
		fprintf(stderr,"usage: dgsend host port 'message'\n");
		exit(1);
	}
	msg = av[3];

    /* get a datagram socket */

    // 建立客户端的socket 很简单，就一句话: socket(PF_INET, SOCK_DGRAM, 0);
	if( (sock = make_dgram_client_socket()) == -1 )
		oops("cannot make socket",2);

    /* combine hostname and portnumber of destination into an address */

    // 通过命令行传进来的服务器ip,port制作服务器的saddr
	if ( make_internet_address(av[1], atoi(av[2]), &saddr) == -1 )
		oops("make addr",4);

    /* send a string through the socket to that address */

    // 也不需要需要建立连接，直接就向saddr发就行了。这里saddr是服务器的地址，sock是客户端的sock
    // 仅仅是创建socket，并没有做任何与客户端的ip,port的绑定，但是服务器那边却知道客户端的ip,port
    
	if ( sendto(sock, msg, strlen(msg), 0, 
				(struct sockaddr *)&saddr,sizeof(saddr)) == -1)
		oops("sendto failed", 3);
	return 0;
}
