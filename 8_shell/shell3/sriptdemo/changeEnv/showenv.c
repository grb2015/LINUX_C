#include<stdio.h>
extern char **environ;

/* 为什么这个就执行不正确呢?
int main(int argc,char **argv)
{
	int i=0;
	while(argv[i] != NULL)
	{
		printf("argv[%d]=%s\n",i,argv[i]);
		i++;
	}
	return 0;
}
*/

int main()
{
	int i;
	for( i = 0;environ[i] != NULL;i++)
		printf("%s\n",environ[i]);
	return 0;
}
