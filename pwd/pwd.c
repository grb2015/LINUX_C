/*****************************************************************
 * 
 * 	name	:	pwd.c
 * 	author 	:	renbin.guo
 * 	brief	:	the complent of linux cmd 'pwd'
 *
 * 	note	:
 * 			"starts in current directory and recursively
 * 			 climbs up to root of filesystem, prints top part
 * 			 then prints current partuses readdir() to get info
 * 			 about each thing"
 * 			 "总体思路是递归，从当前目录上溯到根目录/ 然后从根目录开始打印"
 *
 * 			 bug: prints an empty string if run from "/"
 *
 * 	histroy	:
 * 			 2017-04-01	renbin.guo  created
 *
 *
 *
 ******************************************************************/


#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>

#define NAME_SIZE 	20

void printfpath(ino_t this_inode);

int main()
{
	printpathto( get_inode(".") );
	putchar('\n');
	return 0;
}
/**********************************************************************
 * 
 * 	程序的主要流程都在这里：
 * 		
 *		通过当前inode,获得当前dir_name ，进入父目录，打印当前dir_name ,递归调用即可。
 *	note:
 *		pwd不会打印当前目录的，所以一开始就会chdir("..")从父目录打印
 *
 *
 * */
void printfpath(ino_t this_inode)
{
	ino_t 	my_inode;
	char 	its_name[NAME_SIZE];
	if( get_inode("..") != this_inode )	//根目录才会相等
	{
		chdir("..");		//进入父目录
		inum_to_name(this_inode,its_name,NAME_SIZE);	//通过inode ,获得dir_name.注意这个inode是它的子目录拥有的inode.所以应该是通过子目录的inode,来获得父目录的dir_name
		my_inode = get_inode(".");	// 获得父目录的inode
		printpathto(my_inode);		// 递归调用
		printf("/%s",its_name);
	}
}
/*
 *
 *
 *	brief: 由inode得到dir_name 
 *	params:	
 *		[in ]  inode_to_find 	输入的inode
 *		[out]  namebuf 		由inode得到的dir_name
 *		[in ]  buflen		名字dir_name的长度
 *
 *
 *
 */
		
void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen) 
{
	
	DIR	*dir_ptr;
	struct  dirent  *direntp;	// DIR　和 dirent 结构有什么不同
					// 在下面可知，dir_ptr

	dir_ptr = opendir(".");		
	if ( dir_ptr == NULL)
	{
		perror(".");
		exit(1);
	}
	
	/*遍历打开的目录，在该目录中寻找特定的目录,注意不是文件*/
	while(( direnttp = readdir( dir_ptr)) != NULL)
	
	
		
		
		
	
	

