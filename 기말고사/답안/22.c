#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *ssu_thread(void *arg);
void add_sum(int loc_sum);
void finish_thread(void);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int finish;
int total_sum;

int main(void)
{
	pthread_t tid;
	int loc_sum = 0;
	int i;

	total_sum = 0;
	finish = 0;

	for (i = 1; i < 10; i++)
		pthread_create(&tid, NULL, ssu_thread, 10 * i + 1);
	
	printf("start : %d, main thread\n", 1);
	
	for(i = 1; i < 11; i++)
		loc_sum += i;

	add_sum(loc_sum);
	while(finish != 10)
		sleep(1);
	finish_thread();
}

void *ssu_thread(void *arg)
{
	int loc_sum = 0;
	int start = arg;
	int i;

	printf("start : %d\n", start);

	for (i = start; i < start + 10; i++)
		loc_sum += i;


	add_sum(loc_sum);
	return NULL;
}

void add_sum(int loc_sum)
{
	pthread_mutex_lock(&mutex);
	total_sum += loc_sum;
	finish++;
	pthread_mutex_unlock(&mutex);
}

void finish_thread(void)
{
	printf("total : %d\n", total_sum);
	pthread_mutex_destroy(&mutex);
	exit(0);
}
