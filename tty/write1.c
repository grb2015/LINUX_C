#include	<stdio.h>
#include	<fcntl.h>
#include	<utmp.h>

/***********************************************************************
 *   write1.c
 *   
 *    	purpose: send messages to another terminal
 *       method: open the other terminal for output then
 *      	 	 copy from stdin to that terminal
 *               usage: write1 username
 *
 *               使用方法: ./write1 grb         不需要直接给出终端名，给出用户名即可
 *
 *               example:
 *               	[root@ tty]# who
 *               	root     pts/0        2017-04-07 21:27 (192.168.238.1)
 *               	root     pts/1        2017-04-07 21:27 (192.168.238.1)
 *               	grb      pts/2        2017-04-07 22:09 (192.168.238.1)
 *
 *               	[root@ tty]# ./write1 grb
 *               	hello grb
 *
 *               	[grb@ ~]$ hello grb
 *
 ************************************************************************/

main( int ac, char *av[] )
{
	int	fd;
	char	buf[BUFSIZ];
	char	*get_tty(), *tty_for_user;

	
	if ( ac != 2 ){                                    /* check args */
		fprintf(stderr,"usage: write0 logname\n");
		exit(1);
	}

	tty_for_user = get_tty( av[1] );                  /* find user  */
	if ( tty_for_user == NULL )
		return 1;

	sprintf(buf, "/dev/%s", tty_for_user);	          /* open device */
	fd = open( buf, O_WRONLY );
	if ( fd == -1 ){
		perror(buf); exit(1);
	}

	while( fgets(buf, BUFSIZ, stdin) != NULL )       /* write to user */
		if ( write(fd, buf, strlen(buf)) == -1 )
			break;
	close( fd );
}

char *
get_tty( char *logname )
/*
 *  * purpose: find the tty at which 'logname' is logged in
 *   * returns: a string or NULL if not logged in
 *    *  errors: does not handle multiple logins 
 *     */
{
	static struct utmp utrec;
	int	      utfd;
	int	      namelen = sizeof( utrec.ut_name );
	char	      *retval = NULL ;

	if ( (utfd = open( UTMP_FILE, O_RDONLY )) == -1 )   /* open utmp */
		return NULL;

	/* look for a line where the user is logged in */
	while( read( utfd, &utrec, sizeof(utrec)) == sizeof(utrec) )
		if ( strncmp(logname, utrec.ut_name, namelen ) == 0 )
		{
			retval = utrec.ut_line ;
			break;
		}

	close(utfd);                                       /* close & go */
	return retval;
}

