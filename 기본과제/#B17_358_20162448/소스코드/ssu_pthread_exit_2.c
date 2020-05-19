#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread(void *arg);

int main(void)
{
	pthread_t tid;

    // 새 스레드 생성
	if (pthread_create(&tid, NULL, ssu_thread, NULL) !=0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}
	sleep(1);
	printf("쓰레드가 완료되기전 main함수가 먼저 종료되면 실행중 쓰레드 소멸\n");
	printf("메인 종료\n");
	exit(0);
}

void *ssu_thread(void *arg)
{
	printf("쓰레드 시작\n");
    // 대기 중 메인함수 종료
	sleep(5);
    // 이 아래 부분은 실행되지 않음
	printf("쓰레드 수행 완료\n");
	pthread_exit(NULL);
	return NULL;
}
