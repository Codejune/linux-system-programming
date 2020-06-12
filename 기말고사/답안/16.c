#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void ssu_signal_handler(int signo);

int main(void)
{
	struct sigaction sig_act;
	sigset_t blk_set;
	pid_t pid;

	sigfillset(&blk_set); // 모든 시그널을 블록 시그널 셋에 추가
	sigdelset(&blk_set, SIGCHLD); // SIGCHLD를 블록 시그널 셋에서 제외

	sigemptyset(&sig_act.sa_mask); // 시그널 셋 초기화
	sig_act.sa_flags = 0; // 시그널 플래그 초기화
	sig_act.sa_handler = ssu_signal_handler; // 시그널 핸들러 등록
	sigaction(SIGCHLD, &sig_act, NULL); // SIGCHLD 시그널에 대한 액션 변경

	printf("before fork\n");
	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	} else if (pid != 0) {
		printf("after fork in parent, suspend...\n");
		sigsuspend(&blk_set);
		printf("after suspend\n");
	} else {
		printf("after fork in child, sleep...\n");
		sleep(3);
	}

	exit(0);
}

void ssu_signal_handler(int signo)
{
	printf("in ssu_signal_handler() function\n");
}

