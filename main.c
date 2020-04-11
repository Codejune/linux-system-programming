#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "ssu_score.h"

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);

int main(int argc, char *argv[])
{
	struct timeval begin_t, end_t; // 시작 시간과 끝나는 시간을 저장하는 변수
	gettimeofday(&begin_t, NULL); // 시작 시간 저장

	ssu_score(argc, argv); // 채점 프로그램 시작

	gettimeofday(&end_t, NULL); // 끝나느 시간 저장
	ssu_runtime(&begin_t, &end_t); // 실행 시간 출력

	exit(0);
}

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t) // 실행 시간 출력
{
	end_t->tv_sec -= begin_t->tv_sec; // 끝난 초에서 시작 초 차감

	if(end_t->tv_usec < begin_t->tv_usec){ // 끝난 마이크로 초가 시작 마이크로초보다 작으면 시간 표기 오류 처리
		end_t->tv_sec--; // 끝난 시간 초에서 1초를 빼고
		end_t->tv_usec += SECOND_TO_MICRO; // 끝난 시간 마이크로초를 1초 추가
	}

	end_t->tv_usec -= begin_t->tv_usec; // 마이크로 초 계산
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);
}
