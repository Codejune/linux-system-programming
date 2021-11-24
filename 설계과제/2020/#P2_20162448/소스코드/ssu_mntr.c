#include "common.h"


int main(void) // 프로세스 운영 메인 함수
{
	pid_t daemon_pid;
	struct timeval begin_t, end_t;

	gettimeofday(&begin_t, NULL); // 측정 시작

	// 프롬프트, 모니터링 프로세스 시작
	if((daemon_pid = fork()) < 0) { 
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	// 자식 프로세스(데몬): 모니터링
	if(daemon_pid == 0) 
		monitoring();
	// 부모 프로세스: 프롬프트
	else 
		prompt();

	// 모니터링 프로세스 종료 (삭제됨)
	//kill(daemon_pid + 1, SIGKILL); // SIGKILL(LINUX), KILL(MAC) == 9

	gettimeofday(&end_t, NULL); // 측정 종료
	ssu_runtime(&begin_t, &end_t); // 런타임 출력 

	exit(0);
}

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
