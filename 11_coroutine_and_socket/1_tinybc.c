/**	tinybc.c	* a tiny calculator that uses dc to do its work
 *  			* demonstrates bidirectional pipes
 *      input looks like number op number which
 *    			  tinybc converts into number \n number \n op \n p
 *     			  and passes result back to stdout
 *      
 *       		+-----------+                +----------+
 *        	stdin  >0           >== pipetodc ====>          |
 *         	        |  tinybc   |                |   dc -   |
 *          	stdout <1           <== pipefromdc ==<          |
 *           		+-----------+                +----------+
 *            
 *             			* program outline
 *              				a. get two pipes
 *               				b. fork (get another process)
 *                				c. in the dc-to-be process,
 *                 					 connect stdin and out to pipes
 *                  					 then execl dc
 *                   				d. in the tinybc-process, no plumbing to do
 *                    					 just talk to human via normal i/o
 *                    					 and send stuff via pipe
 *                     				e. then close pipe and dc dies
 *                                            * note: does not handle multiline answers
 */

/********************************************************************* 
 *  brief:
 *  	本程序是linux自带的计算机bc,bc其实只是解析输入，打印输出,内部的
 *	计算引擎是dc.bc与dc的关系可看做协程.
 *  history:
 *  	2017-05-01 renbin.guo created
 *  note:
 *  	usage:
 *  		gcc 1_tinybc.c
 *
 *  		output:
 *  		
 *  		[root@ 11_coroutine_and_socket]# ./a.out 
 *  		tinybc:2+4
 *  		2 + 4 = 6
 *  		tinybc:4^3
 *  		4 ^ 3 = 64    // pressed ctrl+d
 *  		tinybc:[root@ 11_coroutine_and_socket]# 
 *
 ********************************************************************/ 

#include<stdio.h>

#define oops(m,x)\
{\
	perror(m);\
	exit(x);\
}

int main()
{
	int pid;
	int to_dc[2];
	int from_dc[2];

	// 得到2组管道
	if(pipe(to_dc) == -1 || pipe(from_dc) == -1)
		oops("pipe failed",1);

	if( (pid = fork()) == -1)
		oops("cannot fork",2);

	// 子进程执行be_dc(),这里一定是子进程先执行。因为父进程调用了wait,但是父进程可能先执行be_bc
	if(pid == 0)
		be_dc(to_dc,from_dc);
	// 父进程执行be_bc,然后等待子进程结束
	else
	{
		be_bc(to_dc,from_dc);
		wait(NULL);
	}
}
	
	
void be_dc(int in[2],int out[2])
{
	// 将标准输入指向in[0],也即是从in[0]来输入了
	if( dup2(in[0],0) == -1)
		oops("dc:cannot redirect stdout",4);
	close(in[0]);
	close(in[1]);  // 不会用到管道的写端

	// 将标准输出指向out[1],也即是向out[1]输出了
	if(dup2(out[1],1) == -1)
		oops("dc:cannot redirect stdout",4);
	close(out[1]);
	close(out[0]);  // out[]不会用到读的fd

	// now execl dc with the - option
	execlp("dc","dc","-",NULL);
	oops("cannot run dc",5);
}

void be_bc(int to_dc[2],int from_dc[2])
{
	int num1;
	int num2;
	char operation[BUFSIZ];
	char message[BUFSIZ];
	char *fgets();
	FILE *fpout;
	FILE *fpin;
	FILE *fdopen();

	// to_dc   只需要用到to_dc[1],即向dc写
	// from_dc 只需要用到from_dc[0],即从dc读
	close(to_dc[0]);   
	close(from_dc[1]); 
	fpout = fdopen(to_dc[1],"w");  
	fpin =  fdopen(from_dc[0],"r");  

	if(fpout == NULL || fpin == NULL)
		fatal("error convering pipes to streams");
	// 还有这样的写法!
	// parse input  ,如果输入不为NULL(fgets遇到EOF或'\0'会返回,要结束程序，就按ctrl-D --EOF)
	while( printf("tinybc:"),fgets(message,BUFSIZ,stdin) != NULL)
	{
		if( sscanf(message,"%d%[-+*/^]%d",&num1,operation,&num2) != 3)
		{
			printf("syntax error\n");
			continue;
		}
	
		// \np是什么意思 ?
		//  写入到fpout ,也即是把输入传输给dc
		if( fprintf(fpout,"%d\n%d\n%c\np\n",num1,num2,*operation) == EOF)
			fatal("error writing");
		fflush(fpout);
		// 去看看dc是否处理完成了，完成了就去读结果,没完成fgets就阻塞
		if(fgets(message,BUFSIZ,fpin) == NULL)
			break;
		printf("%d %c %d = %s",num1,*operation,num2,message);
    	}
    	fclose(fpout);
    	fclose(fpin);

}
fatal( char *mess )
{
	fprintf(stderr, "Error: %s\n", mess);
	exit(1);
}

