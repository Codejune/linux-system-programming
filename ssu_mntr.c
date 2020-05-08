#include "common.h"


int main(void) // 프로세스 운영 메인 함수
{
	pid_t daemon_pid;


	// 프롬프트, 모니터링 프로세스 시작
	if((daemon_pid = fork()) < 0) { 
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	// 자식 프로세스: 모니터링
	if(daemon_pid == 0) 
		execl("./monitoring", "", (char*)0);
	// 부모 프로세스: 프롬프트
	else {
		printf("child pid : %d\n", daemon_pid);
		prompt();
	}

	// 모니터링 프로세스 종료(미구현)
	kill(daemon_pid + 1, SIGKILL); // SIGKILL(LINUX), KILL(MAC) == 9
	exit(0);
}
