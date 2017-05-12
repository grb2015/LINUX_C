/* twebserv.c - a threaded minimal web server (version 0.2)
 *    usage: tws portnumber
 * features: supports the GET command only
 *           runs in the current directory
 *           creates a thread to handle each request
 *           supports a special status URL to report internal state
 * building: cc twebserv.c socklib.c -lpthread -o twebserv
 */
 
/*
*   brief : 用线程来实现第12章中的web服务器。
            1. 之前是每一个客户请求都开一个进程，现在开一个线程。
            2. 另外，这个版本中没有实现之前的执行cgi可执行文件的功能，因为暂时还没有学习如何在线程中
            装载一个新的可执行程序映像?            3.自己写了一个ls函数代替系统的ls
            4. 添加一个功能:内部统计。当客户请求
               URL:status时候，服务器将内部的统计
               数据发给客户
            5. 之前所有的线程程序都使用pthread_join来等待
               线程的返回，每个线程都占用系统资源，如果程序
               员忘记使用pthead_join来回收线程。则线程资源无法
               回收。我们这里的web服务器不需要等待处理客户端
               的请求的线程返回，所以不需要等待。可以向
               pthread_create传递一个特殊的参数来实现创建
               不需要返回的线程(独立线程)它执行完了之后，自动释放资源。
    note :
            better:统计的功能使用的是共享变量的机制，但是没有加锁。互斥访问
*
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <dirent.h>
#include <time.h>

/* server facts here */

time_t	 server_started ;
int	 server_bytes_sent;
int	 server_requests;

main(int ac, char *av[])
{
	int 		sock, fd;
	int		*fdptr;
	pthread_t	worker;
	pthread_attr_t	attr;

	void *handle_call(void *);

	if ( ac == 1 ){
		fprintf(stderr,"usage: tws portnum\n");
		exit(1);
	}
	sock = make_server_socket( atoi(av[1]) );
	if ( sock == -1 ) { perror("making socket"); exit(2); }

	/* 设置创建独立线程参数，以及初始化系统统计参数*/
	setup(&attr);	

	/* main loop here: take call, handle call in new thread  */

	while(1){
		fd = accept( sock, NULL, NULL );
		server_requests++;		// 系统统计,新来了一个客户端

		fdptr = malloc(sizeof(int));
		*fdptr = fd;
		// 用新创建的线程来执行handle_call
		pthread_create(&worker,&attr,handle_call,fdptr);
	}
}

/*
 * initialize the status variables and
 * set the thread attribute to detached
 */
setup(pthread_attr_t *attrp)
{
	pthread_attr_init(attrp);
	pthread_attr_setdetachstate(attrp,PTHREAD_CREATE_DETACHED);

	time(&server_started);
	server_requests = 0;
	server_bytes_sent = 0;
}

void *handle_call(void *fdptr)
{
	FILE	*fpin;
	char	request[BUFSIZ];
	int	fd ;

	fd = *(int *)fdptr;
	free(fdptr);				/* get fd from arg  */

	fpin = fdopen(fd, "r");			/* buffer input	*/
	fgets(request,BUFSIZ,fpin);		/* read client request */
	printf("got a call on %d: request = %s", fd, request);
	skip_rest_of_header(fpin);	// 跟之前的	read_til_crnl(fpin);一样

	process_rq(request, fd);		/* process client rq */

	fclose(fpin);
}

/* ------------------------------------------------------ *
   skip_rest_of_header(FILE *)
   skip over all request info until a CRNL is seen
   ------------------------------------------------------ */
skip_rest_of_header(FILE *fp)
{
	char	buf[BUFSIZ];
	while( fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0 )
		;
}

/* ------------------------------------------------------ *
   process_rq( char *rq, int fd )
   do what the request asks for and write reply to fd 
   handles request in a new process
   rq is HTTP command:  GET /foo/bar.html HTTP/1.0
   ------------------------------------------------------ */
process_rq( char *rq, int fd)
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	if ( sscanf(rq, "%s%s", cmd, arg) != 2 )
		return;
	sanitize(arg);
	printf("sanitized version is %s\n", arg);

	if ( strcmp(cmd,"GET") != 0 )
		not_implemented();
	// 客户端请求URL:status，服务器将内部的统计数据发给客户
	else if ( built_in(arg, fd) )
		;
	else if ( not_exist( arg ) )
		do_404(arg, fd);
	else if ( isadir( arg ) )
		do_ls( arg, fd );
	// 这里去掉了处理cgi可执行文件的功能
	else
		do_cat( arg, fd );
}
/*
 * make sure all paths are below the current directory
 */
sanitize(char *str)
{
	char	*src, *dest;

	src = dest = str;

	while( *src ){
		if( strncmp(src,"/../",4) == 0 )
			src += 3;
		else if ( strncmp(src,"//",2) == 0 )
			src++;
		else
			*dest++ = *src++;
	}
	*dest = '\0';
	if ( *str == '/' )
		strcpy(str,str+1);

	if ( str[0]=='\0' || strcmp(str,"./")==0 || strcmp(str,"./..")==0 )
		strcpy(str,".");
}

/* handle built-in URLs here.  Only one so far is "status" */
built_in(char *arg, int fd)
{
	FILE	*fp;

	if ( strcmp(arg,"status") != 0 )
		return 0;
	http_reply(fd, &fp, 200, "OK", "text/plain",NULL);

	fprintf(fp,"Server started: %s", ctime(&server_started));
	fprintf(fp,"Total requests: %d\n", server_requests);
	fprintf(fp,"Bytes sent out: %d\n", server_bytes_sent);
	fclose(fp);
	return 1;
}

http_reply(int fd, FILE **fpp, int code, char *msg, char *type, char *content)
{
	FILE	*fp = fdopen(fd, "w");
	int	bytes = 0;

	if ( fp != NULL ){
		bytes = fprintf(fp,"HTTP/1.0 %d %s\r\n", code, msg);
		bytes += fprintf(fp,"Content-type: %s\r\n\r\n", type);
		if ( content )
			bytes += fprintf(fp,"%s\r\n", content);
	}
	fflush(fp);
	if ( fpp )
		*fpp = fp;
	else
		fclose(fp);
	return bytes;
}

/* ------------------------------------------------------ *
   simple functions first:
        not_implemented(fd)      unimplemented HTTP command
        and do_404(item,fd)     no such object
   ------------------------------------------------------ */
not_implemented(int fd)
{
	http_reply(fd,NULL,501,"Not Implemented","text/plain",
			"That command is not implemented");
}

do_404(char *item, int fd)
{
	http_reply(fd,NULL,404,"Not Found","text/plain",
			"The item you seek is not here");
}

/* ------------------------------------------------------ *
   the directory listing section
   isadir() uses stat, not_exist() uses stat
   ------------------------------------------------------ */
isadir(char *f)
{
	struct stat info;
	return ( stat(f, &info) != -1 && S_ISDIR(info.st_mode) );
}

not_exist(char *f)
{
	struct stat info;
	return( stat(f,&info) == -1 );
}

do_ls(char *dir, int fd)
{
	DIR	      *dirptr;
	struct dirent *direntp;
	FILE	      *fp;
	int	      bytes = 0;

	bytes = http_reply(fd,&fp,200,"OK","text/plain",NULL);
	bytes += fprintf(fp,"Listing of Directory %s\n", dir);

	if ( (dirptr = opendir(dir)) != NULL ){
		while( direntp = readdir(dirptr) ){
			bytes += fprintf(fp, "%s\n", direntp->d_name);
		}
		closedir(dirptr);
	}
	fclose(fp);
	server_bytes_sent += bytes;
}

/* ------------------------------------------------------ *
   functions to cat files here.
   file_type(filename) returns the 'extension': cat uses it
   ------------------------------------------------------ */
char * file_type(char *f)
{
	char	*cp;
	if ( (cp = strrchr(f, '.' )) != NULL )
		return cp+1;
	return "";
}

/* do_cat(filename,fd): sends header then the contents */

do_cat(char *f, int fd)
{
	char	*extension = file_type(f);
	char	*type = "text/plain";
	FILE	*fpsock, *fpfile;
	int	c;
	int	bytes = 0;

	if ( strcmp(extension,"html") == 0 )
		type = "text/html";
	else if ( strcmp(extension, "gif") == 0 )
		type = "image/gif";
	else if ( strcmp(extension, "jpg") == 0 )
		type = "image/jpeg";
	else if ( strcmp(extension, "jpeg") == 0 )
		type = "image/jpeg";

	fpsock = fdopen(fd, "w");
	fpfile = fopen( f , "r");
	if ( fpsock != NULL && fpfile != NULL )
	{
		bytes = http_reply(fd,&fpsock,200,"OK",type,NULL);
		while( (c = getc(fpfile) ) != EOF ){
			putc(c, fpsock);
			bytes++;
		}
		fclose(fpfile);
		fclose(fpsock);
	}
	server_bytes_sent += bytes;
}
