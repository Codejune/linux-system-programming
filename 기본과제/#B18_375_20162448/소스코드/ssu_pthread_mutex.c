#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_loop1(void *arg);
void *ssu_loop2(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_value;

int main(void)
{
	pthread_t tid1, tid2;
	int status;

	shared_value = 0;

	// 새 스레드 생성
	if (pthread_create(&tid1, NULL, ssu_loop1, NULL) != 0)
	{
		fprintf(stderr,  "pthread_create error\n");
		exit(1);
	}

	sleep(1);

	// 새 스레드 생성
	if (pthread_create(&tid2, NULL, ssu_loop2, NULL) != 0)
	{
		fprintf(stderr,  "pthread_create error\n");
		exit(1);
	}

	// 스레드 1 종료 대기
	if (pthread_join(tid1, (void *)&status) != 0)
	{
		fprintf(stderr, "pthread_join error\n");
		exit(1);
	}

	// 스레드 2 종료 대기
	if (pthread_join(tid2, (void *)&status) != 0)
	{
		fprintf(stderr, "pthread_join error\n");
		exit(1);
	}

	// mutex 변수 해제
	status = pthread_mutex_destroy(&mutex);
	printf("code = %d\n", status);
	printf("programming is end\n");
	exit(0);
}
void *ssu_loop1(void *arg)
{
	int i;

	for (i=0;i<10;i++)
	{
		// mutex 변수 잠금
		pthread_mutex_lock(&mutex);
		printf("loop1 : %d\n", shared_value);
		// 변수 증가
		shared_value++;
		// 10에 도달시 종료
		if (i == 10)
			return NULL;
		// mutex 변수 해제
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return NULL;
}
void *ssu_loop2(void *arg)
{
	int i;
	for (i=0;i<10;i++)
	{
		// mutex 변수 잠금
		pthread_mutex_lock(&mutex);
		printf("loop2 : %d\n", shared_value);
		// 변수 증가
		shared_value++;
		// mutex 변수 해제
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return NULL;
}
