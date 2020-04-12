#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2=PTHREAD_COND_INITIALIZER;

int length;
int buf[100];

void *ssu_thread_producer(void *arg){
	int i;

	pthread_mutex_lock(&mutex);
	for(i=1;i<=300;i++){
		pthread_cond_wait(&cond1,&mutex);
		//pthread_cond_signal(&cond1);
		printf("1");

	}

	pthread_mutex_unlock(&mutex);
}

void *ssu_thread_consumer(void *arg){
	int i;
	int sum=0;
	
	pthread_mutex_lock(&mutex);

	//pthread_cond_wait(&cond2,&mutex);

	for(i=1;i<=300;i++){
		printf("2");
		
	}

	pthread_cond_signal(&cond2);
	//pthread_cond_signal(&cond1);

	printf("%d\n",sum);
	pthread_mutex_unlock(&mutex);
}
int main(void)
{
	pthread_t producer_tid,consumer_tid;
	int status;

	pthread_create(&producer_tid,NULL,ssu_thread_producer,NULL);
	pthread_create(&consumer_tid,NULL,ssu_thread_consumer,NULL);
	pthread_cond_signal(&cond1);
	while(1){
		;
	}
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);

	exit(0);
}
