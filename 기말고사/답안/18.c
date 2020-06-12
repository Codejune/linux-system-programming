#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *ssu_thread(void *arg);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int glo_val;


int main(void)
{
	pthread_t tid1, tid2;

	glo_val = 1; // 전역변수 초기화

	pthread_create(&tid1, NULL, ssu_thread, (void *) 1);
	pthread_create(&tid2, NULL, ssu_thread, (void *) 2);
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_mutex_destroy(&lock);
	printf("final value : %d\n", glo_val);
	exit(0);
}

void *ssu_thread(void *arg)
{
	int num = arg;

	while (glo_val < 10) {

		pthread_mutex_lock(&lock);
		printf("global value ssu_thread%d : %d\n", arg, glo_val);
		if(glo_val < 10)
			glo_val++;

		pthread_mutex_unlock(&lock);
	}
	return NULL;
}
