#include "ssu_mntr.h"


int main(void) // 프로세스 운영 메인 함수
{
	pid_t mntr_pid;
	
	// 프롬프트, 모니터링 프로세스 시작
	if((mntr_pid = fork()) == 0) // 자식 프로세스 : 모니터링 실행
		mntr_process();
	else
		ssu_mntr(); // 부모 프로세스: 프롬프트 실행

	// 모니터링 프로세스 종료
	kill(mntr_pid, SIGKILL); // SIGKILL(LINUX), KILL(MAC) == 9

	exit(0);
}
