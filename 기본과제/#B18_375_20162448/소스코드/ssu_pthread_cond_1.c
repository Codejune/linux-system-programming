#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int count = 0;
int input = 0;
int t1 = 0, t2 = 0;

int main(void)
{
	pthread_t tid1, tid2;
	int status;

	// 새 스레드 생성
	if (pthread_create(&tid1, NULL, ssu_thread1, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	// 새 스레드 생성
	if (pthread_create(&tid2, NULL, ssu_thread2, NULL) != 0)
	{
		fprintf(stderr, "pthread_create error\n");
		exit(1);
	}

	while (1)
	{
		printf("2개 이상의 개수 입력 : ");
		scanf("%d", &input);
		if (input>=2)
		{
			pthread_cond_signal(&cond1);
			break;
		}
	}

	// thread1이 끝날때 까지 대기
	pthread_join(tid1, (void*)&status);
	// thread2가 끝날때 까지 대기
	pthread_join(tid2, (void*)&status);

	printf("complete \n");
	exit(0);
}

void *ssu_thread1(void *arg)
{
	while (1)
	{
		// 시그널이 도착할때 까지 대기
		pthread_mutex_lock(&mutex1);
		if (input <2)
			pthread_cond_wait(&cond1, &mutex1);

    // 카운트가 input만큼 도달하면 thread2를 시작하고 이 스레드를 종료한다.
		if (input == count)
		{
			pthread_cond_signal(&cond2);
			break;
		}
		// 피보나치 시작
		if(count == 0)
		{
			t2++;
			count++;
			printf("Thread 1 : %d\n", t1);
		}
		// 짝수번 피보나치
		else if (count %2 == 0)
		{
			t1+=t2;
			count++;
			printf("Thread 1 : %d\n", t1);
		}
		pthread_cond_signal(&cond2);
		pthread_cond_wait(&cond1, &mutex1);
		pthread_mutex_unlock(&mutex1);
	}
	return NULL;
}

void *ssu_thread2(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex2);
		if (input<2)
			pthread_cond_wait(&cond2, &mutex2);
		// 카운트가 input만큼 도달하면 thread1을 시작하고 이 스레드 종료
		if (input==count)
		{
			pthread_cond_signal(&cond1);
			break;
		}
		// 두번쌔 피보나치 시작
		/*second term of fibonacci*/
		if(count==1)
		{
			count++;
			printf("Thread 2 : %d\n", t2);
		}
		// 홀수번 피보나치
		else if (count%2 ==1)
		{
			t2+=t1;
			count++;
			printf("Thread 2 : %d\n", t2);
		}
		pthread_cond_signal(&cond1);
		pthread_cond_wait(&cond2, &mutex2);
		pthread_mutex_unlock(&mutex2);
	}
	return NULL;
}
