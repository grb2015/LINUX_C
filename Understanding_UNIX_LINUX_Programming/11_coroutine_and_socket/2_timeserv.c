/* timeserv.c - a socket-based time of day server
 */
/******************************************************
 *    brief:  a socket-based time of day server
	      一直监听客户的连接,客户端连接后，发送服务器上的时间给客户端，然后关掉这路连接，继续监听。
	      除非 kill pid  杀死服务器进程

 *    history: 2017-05-01 renbin.guo created
 *    note:
 *    	usage:
		gcc 2_timeserv.c -o 2
		./2		
 *    		telnet 'hostname' 13000  // 客户端连接
 *    			
 *    	bug #7
 *    		[root@ 11_coroutine_and_socket]# ./2
 *    		Segmentation fault (core dumped)
 *    		[root@ 11_coroutine_and_socket]# 
 *    		
 *    		启动服务器出错

		但是Ubuntu上运行正确！

		1.启动服务器：	
		grb-sim@xxx:~/github/LINUX_CMD/11_coroutine_and_socket$ ./2


		2.在另外一个终端作为客户端连接：
		grb-sim@xxx:~/github/LINUX_CMD/11_coroutine_and_socket$ hostname
		xxx
		grb-sim@xxx:~/github/LINUX_CMD/11_coroutine_and_socket$ telnet xxx 13000
		Trying 127.0.1.1...
		Connected to xxx.
		Escape character is '^]'.
		The time here is ..Tue May  2 13:44:18 2017
		Connection closed by foreign host.
		grb-sim@xxx:~/github/LINUX_CMD/11_coroutine_and_socket$ 
	
		3.服务器的变化
		grb-sim@xxx:~/github/LINUX_CMD/11_coroutine_and_socket$ ./2
		Wow! got a call!
		
 *
 ****************************************************/

#include  <stdio.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>

#define   PORTNUM  13000   /* our time service phone number */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	// 注意这个结构,用来承接gethostbyname的返回值
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */
	FILE	*sock_fp;              /* use socket as stream   */
	char    *ctime();              /* convert secs to string */
	time_t  thetime;               /* the time we report     */

      /*
       * Step 1: ask kernel for a socket
       */

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );    /* get a socket */
	if ( sock_id == -1 ) 
		oops( "socket" );

      /*
       * Step 2: bind address to socket.  Address is host,port
       */

	bzero( (void *)&saddr, sizeof(saddr) ); /* clear out struct     */

	// 先通过gethostname查找本机对于的hostname,然后通过hostname得到ip

	gethostname( hostname, HOSTLEN );       /* where am I ?         */
	hp = gethostbyname( hostname );         /* get info about host  */
	                                        /* fill in host part    */
	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);        /* fill in socket port  */
	saddr.sin_family = AF_INET ;            /* fill in addr family  */

	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

      /*
       * Step 3: allow incoming calls with Qsize=1 on socket
       */

	if ( listen(sock_id, 1) != 0 ) 
		oops( "listen" );

      /*
       * main loop: accept(), write(), close()
       */

	while ( 1 ){
	       sock_fd = accept(sock_id, NULL, NULL); /* wait for call */
		printf("Wow! got a call!\n");
	       if ( sock_fd == -1 )
		       oops( "accept" );       /* error getting calls  */

	        // renbin.guo add comment  2017-05-21,  注意这里int fd 转为了FILE fd  (sock_fp),然后向FILE fd中写
	        // 而注意到2_time_clien.c中确用的是read,wrire系统调用
	        
	       sock_fp = fdopen(sock_fd,"w");  /* we'll write to the   */
	       if ( sock_fp == NULL )          /* socket as a stream   */
		       oops( "fdopen" );       /* unless we can't      */

	       thetime = time(NULL);           /* get time             */
					       /* and convert to strng */
	       fprintf( sock_fp, "The time here is .." );
		// 注意这里不是我们通常用的write系统调用,而是使用的C库函数fprintf()
	       fprintf( sock_fp, "%s", ctime(&thetime) ); 
	       fclose( sock_fp );              /* release connection   */
	}
}
