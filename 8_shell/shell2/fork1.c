// fork 运行一次返回两次，子进程返回0，父进程返回子进程的pid
// bug 这程序结果运行有点不对 /
// 应该是我计算机某些环境出问题了,程序本身没问题
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t  pid;
	
	printf("Process Creation Study\n");
	pid = fork();
	
	switch(pid) {
		case 0:
			printf("Child process is running,CurPid is %d,ParentPid is %d\n", pid, getppid());
			break;
		case -1:
			perror("Process creation failed\n");
			break;
		default:
			printf("Parent process is running,ChildPid is %d,ParentPid is %d\n", pid, getpid());
			break;
	}

	exit(0);
}
