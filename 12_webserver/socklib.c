/*
 **	socklib.c
 **
 **	This file contains functions used lots when writing internet
 **	client/server programs.  The two main functions here are:
 **
 **	make_server_socket( portnum )	returns a server socket
 **					or -1 if error
 **      make_server_socket_q(portnum,backlog)
 **
 **	connect_to_server(char *hostname, int portnum)
 **					returns a connected socket
 **					or -1 if error
 **/ 

/***************************************************************************
 *	brief : 编写网络程序常用的函数的一些包装
 *
 * 	history:2017-05-02 renbin.guo created
 *
***************************************************************************/


#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<time.h>
#include	<strings.h>

#define HOSTLEN 256
#define BACKLOG 1

/**********************************
 * breif : 同make_server_socket,但是指定同时允许请求的客户端数为默认值BACKLOG
 * 
 * returns : socket_id   成功
 * 	     -1 失败             
 *
 * *******************************/
int makeserver_socket(int portnum)
{
	return make_server_socket_q(portnum,BACKLOG);
}

/******************************************
 *  brief  : 完成创建服务器套接字-->转为监听套接字的过程
 *  param  : [in] int portnum -> 端口号
 *  	     [in] backlog  允许排队的请求数
 *  returns: 返回错误 -> -1
 *  	     成功 -> socket_id(已转为监听套接字的)
 *
******************************************/

int make_server_socket_q(int portnum,int backlog)
{
	struct	sockaddr_in saddr;
	struct 	hostent *hp;	// 为了承接gethostbyname()的返回值
	char	hostname[HOSTLEN];
	int	sock_id;
	
	// 创建socket
	sock_id = socket(PF_INET,SOCK_STREAM,0);
	if(sock_id == -1)
		return -1;

	/* 填充套接字 */
	// 现在一般都用memset
	bzero((void*)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);
	
	// 类似memcpy
	bcopy((void*)hp->h_addr,(void*)saddr.sin_addr,hp->h_length);
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AT_INET;

	// 将sock 与address 绑定
	if( bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr)) != 0)
		return -1;
	//转为监听套接字
	if(listen(sock_id,backlog) != 0)
		return -1;

	return sock_id;
}
	
/***************************************************
 * brief : 创建socket,然后连接到host,portnum指定的服务器进程
 * param : [in] host  	要连接的服务器的主机名
 * 	   [in] portnum 要连接的服务器的端口号
 * returns：  -1   : 连接失败
 *   int   sock_id : 连接成功
 *
 *
 *
***************************************************/

int connect_to_server(char *host,int portnum)
{
	int sock;
	struct sockaddr_in servadd;
	struct hostent *hp;
	
	sock = socket(AT_INET,SOCK_STREAM,0);
	if( sock == -1)
		return -1;
	bzero( &servadd, sizeof(servadd));
	hp = gethostbyname(host);
	if(hp == NULL)
		return -1;
	// 这里就没有向上面的强转为(void*),而是将servadd.sin_addr指针显示标志为(struct sockaddr *)类型
	bcopy(hp->h_addr,(struct sockaddr *)servadd.sin_addr,hp->h_length);
	servadd.sin_port = htons(portnum);
	servadd.sin_family = AT_INET;
	
	// sockaddr_in 类型强转为sockaddr类型
	if( connect(sock,(struct sockaddr*)&servadd,sizeof(servadd)) != 0)
		return -1;
	return sock;
}
	
	

	
	
	

	
	
