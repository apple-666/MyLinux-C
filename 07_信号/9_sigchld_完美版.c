/*
 * 提前做 未决信号集 阻塞signal信号。来解决僵尸问题
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


void waitchild(int signo){
	printf("发送一次sigchld信号，调用一次waitchild\n");
	pid_t wpid;
	while(1){ // 可能一次信号 要回收多个子进程
		wpid = waitpid(-1,NULL,WNOHANG);
		if(wpid>0){
			printf("子进程结束 子进程pid:%d\n",wpid);
		}else if(wpid == 0) {
			printf("子进程还活着\n");
			break;
		}else {
			printf("子进程全部结束了\n");
			break;
		}
	}
}
	
int main()
{
	int i = 0;
	int n = 3;
	
	// 阻塞sigchld信号
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGCHLD);
	sigprocmask(SIG_BLOCK,&mask,NULL);	
	
	for(i=0; i<n; i++)	
	{
		//fork子进程
		pid_t pid = fork();
		if(pid<0) //fork失败的情况
		{
			perror("fork error");
			return -1;
		}
		else if(pid>0) //父进程
		{
			printf("father: fpid==[%d], cpid==[%d]\n", getpid(), pid);
		}
		else if(pid==0) //子进程
		{
			printf("child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
			break;
		}
	}

	//父进程
	if(i==3)
	{
		printf("[%d]:father: fpid==[%d]\n", i, getpid());
	
		sleep(5); // 模拟出现僵尸进程
	
		// 注册sigchld
		struct sigaction act;
		act.sa_handler = waitchild;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD,&act,NULL);
		
		// 到父进程创建了 注册了wait后 解除对信号sigchld的阻塞
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
	
		while(1){
			sleep(1);
		}
	}

	//第1个子进程
	if(i==0)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		sleep(1);
	}

	//第2个子进程
	if(i==1)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		sleep(2);
	}

	//第3个子进程
	if(i==2)
	{
		printf("[%d]:child: cpid==[%d]\n", i, getpid());
		sleep(3);
	}

	return 0;
}
