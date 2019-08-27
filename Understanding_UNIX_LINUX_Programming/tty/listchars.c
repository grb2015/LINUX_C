/* buffered input*/

/*
 *	输入： hello 
 *	它不会输入一个就打印一个，而是按行缓冲，按下enter后才打印
 * 
 */



#include<stdio.h>
int main()
{
	char c;
	while( (c=getchar()) != 'Q')
		printf("%c",c);
}


