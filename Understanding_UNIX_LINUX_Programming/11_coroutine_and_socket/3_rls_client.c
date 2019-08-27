/* rls.c - a client for a remote directory listing service
 *         usage: rls hostname directory
 */

/**************************************************************
 *  breif :  远程ls的客户端程序. 可向服务器请求读取远程计算机的某个目录
 *
 *  history:  2017-05-02 renbin.guo created
 *
 *  usage:
 *  [root@localhost 11_coroutine_and_socket]# hostname
 *  localhost.localdomain
 *  [root@localhost 11_coroutine_and_socket]# ./3client localhost.localdomain  /usr/src/
 *  debug
 *  kernels
 *
 *  	
 *
**************************************************************/
#include	<stdio.h>
#include       <sys/types.h>
#include       <sys/socket.h>
#include       <netinet/in.h>
#include       <netdb.h>

#define        oops(msg)       { perror(msg); exit(1); }
#define		PORTNUM		15000

main(int ac, char *av[])
{
	struct sockaddr_in  servadd;        /* the number to call */
	struct hostent      *hp;            /* used to get number */
	int    sock_id, sock_fd;            /* the socket and fd  */
	char   buffer[BUFSIZ];              /* to receive message */
	int    n_read;                      /* for message length */

	if ( ac != 3 ) exit(1);

     /** Step 1: Get a socket **/

	sock_id = socket( AF_INET, SOCK_STREAM, 0 );    /* get a line   */
	if ( sock_id == -1 ) 
		oops( "socket" );          		/* or fail      */

     /** Step 2: connect to server **/

	bzero( &servadd, sizeof(servadd) );     /* zero the address     */
	hp = gethostbyname( av[1] );            /* lookup host's ip #   */
	if (hp == NULL) 
		oops(av[1]);            	/* or die               */
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(PORTNUM);      /* fill in port number  */
	servadd.sin_family = AF_INET ;          /* fill in socket type  */

	if ( connect(sock_id,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
	       oops( "connect" );

     /** Step 3: send directory name, then read back results **/


	if ( write(sock_id, av[2], strlen(av[2])) == -1)
		oops("write");
	// 为什么要发'\n'过去 ? 因为服务器读取客户端的函数用的是fgets，它遇到'\n'就返回，所以协议一次只处理一行
	if ( write(sock_id, "\n", 1) == -1 )
		oops("write");

	while( (n_read = read(sock_id, buffer, BUFSIZ)) > 0 )
		if ( write(1, buffer, n_read) == -1 )  // 向屏幕输出
			oops("write");
	close( sock_id );    
}
