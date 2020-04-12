#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

int lengh;
int buf[10];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2= PTHREAD_COND_INITIALIZER;

void* ssu_thread_producer(void* arg){
	int i;

	for(i=1;i<=300;i++){
		pthread_mutex_lock(&mutex);
		buf[lengh]=i;
		lengh++;
		if(lengh>=100){

			lengh=0;
			pthread_cond_signal(&cond2);
			pthread_cond_wait(&cond1,&mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* ssu_thread_consumer(void *arg){
	int i;
	int sum=0;

	for(i=1;i<=300;i++){
		pthread_mutex_lock(&mutex);
		sum+=buf[lengh];
		lengh++;
		if(lengh>=100){
			if(buf[99]==300)
				break;
			lengh=0;
			pthread_cond_signal(&cond1);
			pthread_cond_wait(&cond2,&mutex);
		}
		pthread_mutex_unlock(&mutex);
	}
	printf("%d\n",sum);
}

int main(void)
{
	pthread_t producer_tid,consumer_tid;

	pthread_create(&producer_tid,NULL,ssu_thread_producer,NULL);
	sleep(1);
	pthread_create(&consumer_tid,NULL,ssu_thread_consumer,NULL);

	pthread_join(consumer_tid,NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
	exit(0);
}
