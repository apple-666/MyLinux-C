#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include <semaphore.h>
typedef struct node{
	int data;
	struct node* next;
}NODE;

NODE *head = NULL;

// 定义 信号量 = 多个互斥锁
sem_t prod;
sem_t cons;


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

		// 加生产锁  (每解一个锁要等消费者通知)
		sem_wait(&prod); //为 生产者加锁 -- 。 为0不再生产

		pNode->next = head;
		head = pNode;

		// 减消费锁
		sem_post(&cons);
	}
}
// 消费者线程
void* consumer(void* arg){
	NODE* pNode = NULL;
	while(1){

		// 消费 加锁
		sem_wait(&cons);

		printf("消费消费了 节点：%d\n",head->data);
		pNode = head;
		head = head->next;

		// 生产者 解锁
		sem_post(&prod);
		free(pNode);
		pNode = NULL;
		sleep(1);
	}
}



int main(){
	// 0 创建 生产+消费者线程
	pthread_t thread1;
	pthread_t thread2;
	int ret;

	// 1 初始信号量
	sem_init(&prod,0,3);
	sem_init(&cons,0,3);

	// 2 创建生产者+消费者线程
	ret = pthread_create(&thread1,NULL,producer,NULL);
	ret = pthread_create(&thread2,NULL,consumer,NULL);

	// 3 join 等待线程结束	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	// 4 释 信号量
	sem_destroy(&prod);
	sem_destroy(&cons);
	return 0;
}
