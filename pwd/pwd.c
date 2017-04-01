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

#define NAME_SIZE 	21	// filename 最多只能20个字节，最后一个字节要存放'\0'

void printpathto(ino_t this_inode);
ino_t get_inode(char *fname);
void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen);

int main()
{
	printpathto( get_inode(".") );
	putchar('\n');
	return 0;
}



/**********************************************************************
 * 
 * 	name	:	printpathto	
 * 	author	: 	renbin.guo		
 * 	brief	:	通过当前目录的inode,进入父目录,打印其父目录的dir_name 
 * 	params	:	
 * 			[in] ino_t   this_inode	: 当前目录的inode
 *
 *	note	:	
 *			1.递归调用
 *			2.pwd不会打印当前目录的，所以一开始就会chdir("..")从父目录打印
 *			
 *
 *
 * ***********************************************************************/
void printpathto(ino_t this_inode)
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



/***********************************************************************
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
 ***********************************************************************/
		
void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen) 
{
	
	DIR	*dir_ptr;
	struct  dirent  *direntp;	// DIR　和 dirent 结构有什么不同
					// 在下面可知，dir_ptr为opendir后的fd,而dirent为readdir的一个entry

	dir_ptr = opendir(".");		
	if ( dir_ptr == NULL)
	{
		perror(".");
		exit(1);
	}
	
	/*遍历打开的目录，在该目录中寻找特定的目录,注意不是文件*/
	while(( direntp = readdir( dir_ptr)) != NULL)
		if( direntp->d_ino == inode_to_find)
		{
			/*
 			* about :strncpy   
  			* 	这里为什么是buflen，而不是sizeof(direntp->d_name) ?
  			*
  			*       1.buflen是一个固定了的，万一d_name没有那么长怎么办?
  			*	答：以NULL填充dest直到复制完buflen个字节
  			*
  			*	2.因为如果d_name过长namebuf不够的话，那也只拷贝buflen,不然拷贝到其后面的内存，溢出。
 			*/
			strncpy(namebuf,direntp->d_name,buflen);	
									
			namebuf[buflen-1] = '\0';		// 这里不管最后一个元素是否有值，都要覆盖为'\0'
			closedir( dir_ptr);
			return ;
		}
	fprintf(stderr,"error looking for inum %d\n",inode_to_find);
	exit(1);
}	
/************************************************************************
 *   name	:	get_inode
 *   author	: 	renbin.guo	
 *   brief	:	returns inode number of the file
 *
 *   params	:	[in] char* fname	:	filename
 *
 *   returns	:	ino_t :	the inode of fname
 *
 *   history	:
 *   			2017-04-01	renbin.guo created
 *
 *   note	:	null
 *   			
 *
 ***********************************************************************/
ino_t get_inode(char *fname)
{
	struct stat info;
	if( stat( fname,&info)==-1)
	{
		fprintf(stderr,"cannot stat");
		exit(1);
	}
	return info.st_ino;
}
	
	
	
		
		
		
	
	

