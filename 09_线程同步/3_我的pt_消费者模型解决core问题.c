#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

typedef struct node{
	int data;
	struct node* next;
}NODE;

NODE *head = NULL;

// 定义锁 + 条件变量
pthread_mutex_t mutex;
pthread_cond_t cond;

// 生产者线程
void* producer(void* arg){
	NODE *pNode = NULL;
	while(1){
		// 初始化商品
		pNode = (NODE*)malloc(sizeof(NODE));
		if(pNode==NULL){
			perror("malloc error");
			exit(-1);
		}
		pNode->data = rand()%1000;
		printf("生产者生产出 node:%d\n",pNode->data);

		// 加锁
		pthread_mutex_lock(&mutex);

		pNode->next = head;
		head = pNode;

		// 解锁
		pthread_mutex_unlock(&mutex);

		// 通知消费者
		pthread_cond_signal(&cond);

		sleep(1);
	}
}
// 消费者线程
void* consumer(void* arg){
	NODE* pNode = NULL;
	while(1){

		// 消费 加锁
		pthread_mutex_lock(&mutex);
		if(head==NULL){
			// 若不满足 则阻塞+解锁。不会继续执行
			// 在不满足之后，若满足 解除阻塞+加锁。
			// 这里出现两个阻塞：cond不满足造成阻塞1 加不了锁造成阻塞2
			// 为什么会造成core掉问题：多个消费者同时解除了阻塞1.造成head->data访问数据为空
			pthread_cond_wait(&cond,&mutex);
		}

		if(head==NULL){ // 再次判断解决core问题
			pthread_mutex_unlock(&mutex);
			continue;
		}
		printf("消费消费了 节点：%d\n",head->data);
		pNode = head;
		head = head->next;

		// 解锁
		pthread_mutex_unlock(&mutex);
		free(pNode);
		pNode = NULL;
		sleep(1);
	}
}



int main(){
	// 0 创建 生产+消费者线程
	pthread_t thread1[3];
	pthread_t thread2[3];
	int ret;

	// 1 初始化互斥锁+条件变量
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

	// 2 创建生产者+消费者线程
	int i = 0;
	for(i = 0;i<3;i++){
		ret = pthread_create(&thread1[i],NULL,producer,NULL);
		ret = pthread_create(&thread2[i],NULL,consumer,NULL);
	}

	// 3 join 等待线程结束	
	for(i = 0;i<3;i++){

		pthread_join(thread1[i],NULL);
		pthread_join(thread2[i],NULL);
	}

	// 4 释放锁+条件变量
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
