/************************************************************
 * 
 *   name:	ls.c
 *   author:	renbin.guo
 *   brief:	the 2ed verion of ls
 *   note:
 *   		if no args,use . 
 *   		bug:	
 *   			can't print give dirent.like  ./ls   /mnt  
 *   history:	
 *   		2017-03-30 	renbin.guo created
 *   		2017-03-31	renbin.guo modify creat verion 2	implement ls -l	
 *   		2017-04-01	fixed bug2 :add output format control (输出格式控制)
 *
 ***************************************************************/
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>

// renbin.guo added for "ctime"	,fixed the ctime run bug at  #1
#include<time.h>
char* gid_to_name(gid_t gid);
void mode_to_letters(int mode,char str[]);
char * uid_to_name(uid_t uid);
void mode_to_letters(int mode,char str[]);
void show_file_info(char *filename,struct stat *info_p);
void dostat(char *filename);
/*打印当前目录下的所有文件*/
void do_ls(char []);

int main(int ac,char *av[])
{
	if(ac == 1)
		do_ls(".");
	else
		while(--ac)	// 第一个参数为ls 所以要--ac,++av	
		{
			printf("%s:\n",*++av);
			do_ls(*av);
		}
	return 0;
}
/*********************************************************************
 *	 name:		do_ls
 *	 auther:	renbin.guo
 *	 params:	[in] dirname:    direct name 
 *	 breif:		printf all files in dirname
 *	 note:	
 *	 history:
 *	 		2017-03-30	renbin.guo created
 * 
 ********************************************************************/

void do_ls(char dirname[])
{
	DIR 	*dir_ptr;
	struct 	dirent *direntp;
	dir_ptr = opendir(dirname);
	if(dir_ptr == NULL)
		fprintf(stderr,"ls : cannot open %s \n",dirname);
	else
	{
		while( (direntp = readdir(dir_ptr)) != NULL)
		//	printf("%s\n",direntp->d_name);		version one
			dostat(direntp->d_name);
		closedir(dir_ptr);
	}
}
/*********************************************************************
 *         name:          dostat 
 *         auther:        renbin.guo
 *         params:        [in] filename  
 *         breif:         print all the file attributese of 'filename'        
 *         note:  
 *         history:
 *                              2017-03-31      renbin.guo created
 *          
 ***************************************************************************/
void dostat(char *filename)
{
	struct stat info;
	if( stat(filename,&info) == -1)
		perror(filename);
	else
		show_file_info(filename,&info);
}
/*********************************************************************
 *           name:          show_file_info
 *           auther:        renbin.guo
 *
 *           params:        [in] filename    only for print file name(仅仅为了打印，不作为其他用途)
 *           		    [in] info_p	     the stat information of 'filename'(filename的属性)
 *
 *           breif:         print all the file attributese of 'filename'(打印文件属性)      
 *		            for example: -rw-r--r--.  1 root root    41364 Jan 20 19:47 install.log
 *		            (以字符串输出，比如用户名为root 而不是显示0 )
 *
 *           note:          
 *           history:       
 *                           2017-03-31      renbin.guo created
 *                                 
 * ***************************************************************************/

void show_file_info(char *filename,struct stat *info_p)
{
	//char 	*ctime();	//if comment this ,you need to add time.h (书上有这样的声明，如果不声明的话需要加上头文件 time.h 不然运行会有错误)
				// 但是为什么是char *ctime() ?参数表不正确啊?
	char	 modestr[11];
	/*将数字表示的mode信息转成字符打印*/
	mode_to_letters(info_p->st_mode,modestr);
	printf("%s",modestr);

	printf("%4d ",(int)info_p->st_nlink);		//format: 右对其，至少显示4位数字,不足的补空格

	/*同样，uid和gid也需要转换*/
	printf("%-8s", uid_to_name(info_p->st_uid));	// 左对齐，至少显示8个字符，不足的补空格	
	printf("%-8s  ", gid_to_name(info_p->st_gid));	// 左对齐，至少显示8个字符，不足的补空格
	
	printf("%8ld ",info_p->st_size);		// 右对齐，至少显示8位数字，不足的补空格
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));  // 右对其，只显示12个字符
	printf("%9s\n",filename);			// renbin.guo modify 2017-04-01 左对齐，至少显示6个字符，不足补空格
}


/*
 *	utility functions 
 *
 * 
 */
void mode_to_letters(int mode,char str[])
{
	strcpy(str,"----------");		/*default attribute d_rwx_rwx_rwx */ 
	if(S_ISDIR(mode))	str[0] = 'd';	/* 	directory 	   	*/
	if(S_ISCHR(mode))	str[0] = 'c';	/* 	char device		*/
	if(S_ISBLK(mode))	str[0] = 'b';	/* 	block device		*/

	/*	3 bits for user		*/

	if( mode & S_IRUSR )	str[1] = 'r';
	if( mode & S_IWUSR )	str[2] = 'w';
	if( mode & S_IXUSR )	str[3] = 'x';

	
	/*	3 bits for group	*/

	if( mode & S_IRGRP )	str[4] = 'r';
	if( mode & S_IWGRP )	str[5] = 'w';
	if( mode & S_IXGRP )	str[6] = 'x';


	/*	3 bits for other	*/

	
	if( mode & S_IROTH )	str[7] = 'r';
	if( mode & S_IROTH )	str[8] = 'w';
	if( mode & S_IROTH )	str[9] = 'x';
}


//#include < pwd.h >
#include"/usr/include/pwd.h"
char * uid_to_name(uid_t uid)
{
	struct	passwd*pw_ptr;
	static  char numstr[10];
	pw_ptr = getpwuid(uid);
	/*	if no string name ,output uid (如果uid没有对应的用户名，则直接输出数字类型uid )	*/
	if(pw_ptr == NULL)
	{
		sprintf(numstr,"%d",uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}


#include <grp.h>

char* gid_to_name(gid_t gid)
{
	struct group*grp_ptr;
	static char numstr[10];		/// 这里两处定义numstr不重复吗
	grp_ptr = getgrgid(gid);
	if(grp_ptr == NULL)
	{
		sprintf(numstr,"%d",gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
