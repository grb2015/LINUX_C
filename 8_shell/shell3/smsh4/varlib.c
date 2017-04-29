#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"varlib.h"

#define MAXVARGS 200

static char *new_string( char *, char *);	/* private methods	*/
static struct var *find_item(char *, int);


static struct var tab[MAXVARGS];

/*
 * 打印tab[]中所有不为空的值
 */
void VLlist()
{
	int	i;
	for(i=0;i<MAXVARGS && tab[i].str != NULL;i++)
	{
		if(tab[i].global)
			printf("* %s\n",tab[i].str);
		else
			printf("%s\n",tab[i].str);
	}
}


int VLstore(char *name,char *var)
{
	//  itemp为var结构类型的指针
	struct var *itemp;
	char *s;
	int rv = 1;
	// find spot to put it  and make new string
	if( (itemp = find_item(name,1))!=NULL && (s=new_string(name,var))!=NULL)
	{
		if(itemp->str)
			free(itemp->str);
		itemp->str = s;
		rv = 0;
	}
	return rv;
}

char *new_string(char *name,char *val)
{
	char *retval;	
		
	retval = malloc(strlen(name)+strlen(val)+2);

	if(retval != NULL)
		sprintf(retval,"%s=%s",name,val);
	return retval;
}

/* 
 *  从全局变量tab中查找是否有name记录，没有的话根据第二个参数来觉得是否创建一个新记录
*/

static struct var *find_item(char *name,int first_blank)
{
	int	i;
	int 	len = strlen(name);
	char 	*s;
	for( i = 0 ; i<MAXVARGS && tab[i].str != NULL ; i++ )
	{
		s = tab[i].str;
		if(strncmp(s,name,len) == 0 && s[len] == '=')
		{
			return &tab[i];
		}
	}
	if(i < MAXVARGS && first_blank)
		return &tab[i];
	return NULL;
}
int VLexport(char*name)
{
        struct var *itemp;
        int rv = 1;
        if( (itemp = find_item(name,0)) != NULL)
        {
                itemp->global = 1;
                rv = 0;
        }
        //为什么这里为""
        else if(VLstore(name,"")  == 1)
        // 注意这里仍然是name作参数，所以不是递归吧，而是再次检测
                rv = VLexport(name);
        return rv;
}

//renbin.guo added for version4

int VLenviron2table(char *env[])
/*
 * initialize the variable table by loading array of strings
 * return 1 for ok, 0 for not ok
 */
{
	int     i;
	char	*newstring;

	for(i = 0 ; env[i] != NULL ; i++ )
	{
		if ( i == MAXVARGS )
			return 0;
		newstring = malloc(1+strlen(env[i]));
		if ( newstring == NULL )
			return 0;
		strcpy(newstring, env[i]);
		tab[i].str = newstring;
		tab[i].global = 1;
	}
	while( i < MAXVARGS ){		/* I know we don't need this	*/
		tab[i].str = NULL ;	/* static globals are nulled	*/
		tab[i++].global = 0;	/* by default			*/
	}
	return 1;
}

char ** VLtable2environ()
/*
 * build an array of pointers suitable for making a new environment
 * note, you need to free() this when done to avoid memory leaks
 */
{
	int	i,			/* index			*/
		j,			/* another index		*/
		n = 0;			/* counter			*/
	char	**envtab;		/* array of pointers		*/

	/*
	 * first, count the number of global variables
	 */

	for( i = 0 ; i<MAXVARGS && tab[i].str != NULL ; i++ )
		if ( tab[i].global == 1 )
			n++;

	/* then, allocate space for that many variables	*/
	envtab = (char **) malloc( (n+1) * sizeof(char *) );
	if ( envtab == NULL )
		return NULL;

	/* then, load the array with pointers		*/
	for(i = 0, j = 0 ; i<MAXVARGS && tab[i].str != NULL ; i++ )
		if ( tab[i].global == 1 )
			envtab[j++] = tab[i].str;
	envtab[j] = NULL;
	return envtab;
}

	
	
	
	
			
