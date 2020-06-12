/**
 * @file 7.c
 * @brief ssu_pthread_cond_3.c
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h>

#define VALUE_DONE 10 
#define VALUE_STOP1 3 
#define VALUE_STOP2 6

void *ssu_thread1(void *arg);
void *ssu_thread2(void *arg);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // 조건변수의 초기화 및 해제
int glo_val = 0;

int main(void) {
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, ssu_thread1, NULL); // ssu_thread1 생성
	pthread_create(&tid2, NULL, ssu_thread2, NULL); // ssu_thread2 생성
	
	pthread_join(tid1, NULL); // ssu_thread1 종료까지 대기
	pthread_join(tid2, NULL); // ssu_thread2 종료까지 대기

	//pthread_mutex_destroy(&lock); // mutex 락 제거
	//pthread_cond_destroy(&cond); // cond 락 제거
	printf("final value: %d\n", glo_val);
	exit(0); 
}

void *ssu_thread1(void *arg) {
		while (1) {
			pthread_mutex_lock(&lock);
			pthread_cond_wait(&cond, &lock); // cond 시그널이 오기 전까지 블록상태로 대기
			glo_val++;
			printf("global value ssu_thread1: %d\n", glo_val);
			pthread_mutex_unlock(&lock);

			if(glo_val >= VALUE_DONE)
				return NULL;

		} 
}

void *ssu_thread2(void *arg) {

	while (1) {
		pthread_mutex_lock(&lock);
		if (glo_val < VALUE_STOP1 || glo_val > VALUE_STOP2)
			pthread_cond_signal(&cond); // 스레드 실행 순서 확인, 및 시그널 전송
		else {
			glo_val++;
			printf("global value ssu_thread2: %d\n", glo_val);
		}
		pthread_mutex_unlock(&lock);

		if(glo_val >= VALUE_DONE) 
			return NULL;

	} 
}
