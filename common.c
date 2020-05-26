#include "common.h"

/**
 * @brief 실행 시간 출력
 * @param begin_t 시작 시간
 * @param end_t 종료 시간
 */
void ssu_runtime(struct timeval *begin_t, struct timeval *end_t) // 실행시간 출력 
{
	end_t->tv_sec -= begin_t->tv_sec;

	if(end_t->tv_usec < begin_t->tv_usec){
		end_t->tv_sec--;
		end_t->tv_usec += SECOND_TO_MICRO;
	}

	end_t->tv_usec -= begin_t->tv_usec;
	printf("Runtime: %ld:%06ld(sec:usec)\n", end_t->tv_sec, end_t->tv_usec);
}
