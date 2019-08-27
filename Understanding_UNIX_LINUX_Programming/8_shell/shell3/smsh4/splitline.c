#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"smsh.h"  // 包含了fatal

#define is_delim(x) (x== ' '|| x == '\t')

/******************************************
 * 	brief:	get next command
 * 		这个分明就是把文件fd中的数据拷贝到buf中啊，哪里是get next command ?
		注意，这里有if(c == '\n'),也即是遇到换行符就结束，所以这个程序的功能的确是
 *		获得一行的输入即一行命令
 * 	param:	
 * 		[in]  char*prompt: 输入的提示符，这里为"> "
 * 		[in]  FILE *fp   : fd文件，存放命令字符
 * 	returns: 
 * 		NULL   -> 文件为空
 * 		char * -> 存放拷贝的文件的buf的地址(在堆中分配)
 * 			
 ********************************************************************/
char * next_cmd(char*prompt,FILE *fp)
{
	char *buf;
	int bufspace = 0;
	int pos = 0;
	int c;
	
	printf("%s",prompt);
	// 我们这里的fp是stdin,从输入中获取,如果不为文件结束符
	while( (c=getc(fp)) != EOF)
	{
		// 如果buf满了
		if(pos+1 >= bufspace)
		{
			if(bufspace == 0)
				buf = emalloc(BUFSIZ);
			else	// 重新分配buf的长度，增加BUFSIZ个字节
				buf = erealloc(buf,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
		}
		// 如果输入的是换行符,代表执行当前行
		if(c == '\n')
			break;
		//将输入的字符存在buf中
		buf[pos++] = c;	 
	}
	// 如果fd中什么内容也没有,即第一个字符为EOF,这种情况返回NULL
	if(c == EOF && pos == 0)
		return NULL;
	buf[pos] = '\0';
	return buf;
}
	
/* 
 *  brief : 动态分配n字节空间 (包含了出错处理的malloc)
 */
void *emalloc(size_t n)
{
	void *rv;
	if((rv = malloc(n)) == NULL)
		fatal("out of memory","",1);
	return rv;
}
		

/*
 *  brief : 将p作为其实地址，分配n字节的空间 (包含了错误处理的realloc)
 */
void*erealloc(void *p,size_t n)
{
	void *rv;
	if( (rv = realloc(p,n)) ==  NULL)
		fatal("readlloc() failed","",1);
	return rv;
}

/*
 * 将字符串line解析为命令行参数,存放在指针数组里返回。
 */
char** splitline(char *line)
{
	char 	*newstr();
	char 	**args;		// 指针数组，存放解析后的参数
	int	spots = 0;
	int 	bufspace = 0;
	int 	argnum = 0;	// 保存参数个数
	char 	*cp = line;
	char 	*start;
	int 	len;
	
	if(line == NULL)
		return NULL;
	args	 = emalloc(BUFSIZ);	// 这里 emalloc的是一个512字节的连续空间
	bufspace = BUFSIZ;
	spots	 = BUFSIZ/sizeof(char *);	// BUFSIZ这么多字节，能存放的指针变量数。(BUFSIZ为512,一个指针4字节，所以能存
						// 103个指针)
	
	while(*cp != '\0')
	{
		while(is_delim(*cp))	// 跳过开头的空格
			cp++;
		if(*cp == '\0')
			break;
		// 空间不够,参数大于了spots(103),一般是不可能的。
		// 为什么是加1,因为最后一个要存放NULL
		if(argnum+1 >= spots)
		{
			// 
			args = erealloc(args,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ/sizeof(char*));
		}
		
		start = cp;
		len = 1; 
		// 现在cp不为空格,循环，直到遇到空格
		while(*++cp != '\0' && !(is_delim(*cp)))
			len ++;
		args[argnum++] = newstr(start,len);	// newstr中会malloc len长度的空间，然后把地址返回给指针args[argnum+]
	}
	args[argnum] = NULL;
	return args;
}

/*
 *  breif  : 开辟一段动态内存，将从start起的连续len个字符拷贝到开辟的内存中
 *  returns: char* 开辟的动态内存
 *  	     程序退出：	动态内存申请失败
 */

char *newstr(char *start,int len)
{
	char *p = emalloc(len+1);	// emlloac已经包含了错误检测,所以放心使用
	strncpy(p,start,len);
	*(p+len) = '\0';
	return p;
}


void  freelist(char **list)
{
	char ** cp = list;
	while(*cp)
	{
		free(*cp);
		cp++;
	}
	freelist(list);
}
/*
 * 错误处理函数
 */
void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}
		
	


	

	
	
		
			
	

