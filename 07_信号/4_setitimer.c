#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
void sighandler(int signo)
{
	printf("signo:%d 发出了信号SIGALRM\n",signo);
}


int main(){

	signal(SIGALRM,sighandler);
	
	struct itimerval tm;
	printf("3s后开始周期发送信号（周期为1s）\n");
	tm.it_value.tv_sec = 3; //触发信号时间
	tm.it_value.tv_usec = 106;
	
	tm.it_interval.tv_sec = 1;	// 周期时间
	tm.it_interval.tv_usec = 100;	
	printf("啊啊啊\n");
	setitimer(ITIMER_REAL,&tm,NULL);
	
	while(1){
		sleep(1);
	}
	//	sleep(20) ; // SIGALRM默认动作是终止 sleep到一半会终止 所有用while才有效
	return 0;
}	
