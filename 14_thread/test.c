#include<stdio.h>

int main()
{
	int c;
	FILE *fd = fopen("file3",'w');
//	while( (c=getchar()) != EOF);
	int repeat=10;
	while(repeat-->0)
	{
		for(c='0';c<='9';c++)
		{
			fputc(c,fd);
			fputc('\',fd);
		}
	}
	fclose(fd);
}

