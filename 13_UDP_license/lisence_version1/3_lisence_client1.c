/****************************************************************************
 * lclnt1.c
 * License server client version 1
 *  link with lclnt_funcs1.o dgram.o
 */

 
/*  brief : 许可证服务器程序客户端
            licence 管理功能: 没有很多人在使用该程序，客户端请求ticket,服务器发送票据给
*           客户端进程，给予许可，如果达到了最大的程序实例数，服务器发送无可用
*           票据的消息给客户，并且通知客户稍后再试或者购买支持功能更多用户版本的
*           软件
*   history : 2017-05-05 renbin.guo created

    note:
    usage:
		gcc 3_lisence_server1*.c dgram.c -o server1
		gcc 3_lisence_client1*.c dgram.c -o client1

		1. server:
			grb-sim@xxx:~/github/LINUX_CMD/13_UDP_license/lisence_version1$ ./server1 
		2. client:
			grb-sim@xxx:~/github/LINUX_CMD/13_UDP_license/lisence_version1$ ./client1 
			CLIENT [2341]: got ticket 2341.0
			SuperSleep version 1.0 Running - Licensed Software
			CLIENT [2341]: released ticket OK 
			grb-sim@xxx:~/github/LINUX_CMD/13_UDP_license/lisence_version1$ ./client1 
			CLIENT [2375]: got ticket 2375.0
			SuperSleep version 1.0 Running - Licensed Software
			CLIENT [2375]: released ticket OK 
			grb-sim@xxx:~/github/LINUX_CMD/13_UDP_license/lisence_version1$ 
		3. server:
			grb-sim@xxx:~/github/LINUX_CMD/13_UDP_license/lisence_version1$ ./server1 
			SERVER: GOT: HELO 2375 
			SERVER: SAID: TICK 2375.0 
			SERVER: GOT: GBYE 2375.0 
			SERVER: SAID: THNX See ya! 
	注意：服务器和客户端都用的是一个主机。所以客户机命令行参数不需要服务器IP/端口号


*
*/   
#include <stdio.h>

int main()
{
    // 进行初始化，比如创建本身UDP套接字、制作服务器的addr等。
	setup();

	// 向服务器请求票据，以访问程序.向服务器发送数据,然后接收服务器的返回。
	// 客户端的目的是运行do_regular_work程序,如果get_ticket()函数返回为0,
	// 则说明当前用户数已经超过了,也就不能运行do_regular_work了
	if (get_ticket() != 0 )
		exit(0);
		
    // 请求票据成功，就做想做的工作
	do_regular_work();

    // 完了之后释放掉票据,也是向服务器发送消息，表明我这边工作做完了，让服务器减少用户数
	release_ticket();

	// 关机退出
	shut_down();

}
/****************************************************************************
 * do_regular_work  the main work of the application goes here
 */
do_regular_work()
{
	printf("SuperSleep version 1.0 Running - Licensed Software\n");
	sleep(10);	/* our patented sleep algorithm */
}

