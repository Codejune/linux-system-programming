#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 5

void *ssu_printhello(void *arg);

int main(void)
{
	pthread_t tid[THREAD_NUM];
	int i;
	for (i=0;i<THREAD_NUM;i++)
	{
		printf("In main: creating thraed %d\n", i);
        // 새 스레드 생성
		if (pthread_create(&tid[i], NULL, ssu_printhello, (void *)&i)!=0)
		{
			fprintf(stderr, "pthread_create error\n");
			exit(1);
		}
	}
    // 스레드 종료
	pthread_exit(NULL);
	exit(0);
}

void *ssu_printhello(void *arg)
{
	int thread_index;

    // 인자 데이터 변환
	thread_index = *((int*)arg);
	printf("Hello World! It's me, thread #%d!\n", thread_index);
    // 스레드 종료
    pthread_exit(NULL);
	return NULL;
}
