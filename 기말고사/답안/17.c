#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

pthread_t glo_tid;

int main(void)
{
	pthread_t loc_tid[2];
	int i;

	// 스레드 생성
	pthread_create(&loc_tid[0], NULL, ssu_thread1, NULL);
	pthread_create(&loc_tid[1], NULL, ssu_thread2, NULL);

	sleep(5);

	for (i = 0; i < 2; i++)
		if (loc_tid[i] == glo_tid)
			printf("second thread assigns it's tid to global tid\n");
	
	exit(0);
}

void *ssu_thread1(void *arg)
{
	printf("in ssu_thread1\n");
	return NULL;
}

void *ssu_thread2(void *arg)
{
	printf("in ssu_thread2\n");
	glo_tid = pthread_self();
	return NULL;
}
