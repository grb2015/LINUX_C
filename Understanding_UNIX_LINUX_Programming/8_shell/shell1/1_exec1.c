/*
 *   brief   : 一个程序如何运行另外一个程序 exec
 *   history :	2017-04-19 renbin.guo created 
 */

int main()
{
	char *argv[3];
	argv[0]= "ls";
	argv[1]= "-l";
	argv[2]= 0;	// 为NULL
	printf("***about to exec ls -l\n");
	// 第一个参数是程序名，第二个参数是程序运行时的参数
	// 注意，运行时的参数最后一个必须为0
	execvp("ls",argv);

	//永远不会执行到这里来，因为当前进程被ls镜像替换了
	printf("nerver go to here\n"); 
}

