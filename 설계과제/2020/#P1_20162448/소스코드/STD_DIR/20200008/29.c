#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VALUE_DONE 10
#define VALUE_STOP1 3
#define VALUE_STOP2 6

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

int glo_val = 0;

int main(void)
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, &ssu_thread1, NULL);
	pthread_create(&tid2, NULL, &ssu_thread2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	//printf("final value: %d\n", glo_val);
	exit(0);
}

void *ssu_thread1(void *arg)
{
	while(1){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&cond, &lock);
		glo_val++;
		printf("global value ssu_thread1: %d\n", glo_val);
		pthread_mutex_unlock(&lock);
		
		if(glo_val >= VALUE_DONE)
			return NULL;
	}
}

void *ssu_thread2(void *arg)
{
	while(1){
		pthread_mutex_lock(&lock);
		if(glo_val < VALUE_STOP1 || glo_val > VALUE_STOP2)
			pthread_cond_signal(&cond);
		else{
			glo_val++;
			printf("global value ssu_thread2: %d\n", glo_val);
		}
		pthread_mutex_unlock(&lock);
		
		if(glo_val >= VALUE_DONE)
			return NULL;
	}
}