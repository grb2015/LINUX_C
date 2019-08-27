/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：4_environment.c
*   breif：	打印系统环境变量
*   history：2017-05-18 renbin.guo created.
*   note：
*   usage：
		[root@grb_host 7_process]# ./4
		env[0] = HOSTNAME=grb_host
		env[1] = SELINUX_ROLE_REQUESTED=
		env[2] = TERM=xterm
		env[3] = SHELL=/bin/bash
		env[4] = HISTSIZE=1000
		env[5] = SSH_CLIENT=192.168.238.1 49619 22
		env[6] = SELINUX_USE_CURRENT_RANGE=
		env[7] = SSH_TTY=/dev/pts/4
		env[8] = USER=root
		env[9] = LS_COLORS=rs=0:di=01;34:ln=01;
		...
*
================================================================*/
#include<stdio.h>
int main(int argc, char const *argv[],char **envir)
{
	int i ;
	for(i = 0; envir[i]!= NULL ;i++)
		printf("env[%d] = %s\n",i,envir[i] );
	return 0;
}




