#include<string.h>
#include<ctype.h>
#include"smsh.h"
#include"varlib.h"

int assing(char *);
int okname(char *);
/*******************************************
*  returns: 1 if args[0] is builtin
*            0 if not 
*
*
*******************************************/

int builtin_command(char **args,int *resultp)
{
	int rv = 0;
	if(strcmp(args[0],"set") == 0)
	{
		VLlist();
		*resultp = 0;
		rv = 1;
	}
	else if(strchr(args[0],"=") != NULL)
	{
		*resultp = assign(args[0]);
		if(*resultp != -1)
			rv = 1;
	}
	else if(strcmp(args[0],"export")== 0)
	{
		if(args[1] !=NULL && okname(args[1]))
			*resultp = VLexport(args[1]);
		else
			*resultp = 1;
		rv = 1;
	}
	return rv;
}

// = 赋值
int assign(char *str)
{
	char *cp;
	int rv;
	cp = strchr(str,"=");
	*cp = '\0';
	// VLstrore 将str中从cp+1位置到末尾的值存放在全局变量tab[]中
	rv = (okname(str)? VLstore(str,cp+1):-1);
	*cp = "=";
	return rv;
}

/*
 * 验证str是不是合法的变量(字母数组和下划线)
 */
int okname(char *str)
{
	char *cp ;
	for(cp = str;*cp;*cp++)
	{
		if(( isdigit(*cp) && cp == str) || !(isalnum(*cp) || *cp == ' '))
			return 0;
	}
	return (cp != str);
}

		
