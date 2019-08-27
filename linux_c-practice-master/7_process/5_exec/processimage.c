/*================================================================
*   Copyright (C) 2017 free for learn.
*   
*   filename：processimage.c
*   breif：  打印传入参数
*   history：2017-05-18 renbin.guo created.
*   note：
*   usage：
*
================================================================*/
#include<stdio.h>
int main(int argc, char const *argv[])
{
	int i;
	for(i = 0;argv[i] != NULL; i++)
		printf("argv[%d] = %s\n",i,argv[i] );
	return 0;
}

