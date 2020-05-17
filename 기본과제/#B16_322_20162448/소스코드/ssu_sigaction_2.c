#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_check_pending(int signo, char *signame);
void ssu_signal_handler(int signo);

int main(void)
{
	struct sigaction sig_act;
	sigset_t sig_set;

	sigemptyset(&sig_act.sa_mask); // 시그널 집합 초기화
	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;

	if (sigaction(SIGUSR1, &sig_act, NULL) != 0) { // SIGUSR1 시그널 처리 등록
		fprintf(stderr, "sigaction() error\n");
		exit(1);
	}
	else {
		sigemptyset(&sig_set); // 시그널 집합 초기화
		sigaddset(&sig_set, SIGUSR1); // 시그널 등록

		if (sigprocmask(SIG_SETMASK, &sig_set, NULL) != 0) { // sig_set procmask..
			fprintf(stderr, "sigprocmask() error\n");
			exit(1);
		}
		else {
			printf("SIGUSR1 signals are now blocked\n");
			kill(getpid(), SIGUSR1); // SIGUSR1 시그널 전달
			printf("after kill()\n");
			ssu_check_pending(SIGUSR1, "SIGUSR1"); // SIGUSR1 시그널 유무 확인
			sigemptyset(&sig_set); // 시그널 집합 초기화
			sigprocmask(SIG_SETMASK, &sig_set, NULL); // 블록 시그널 설정
			printf("SIGUSR1 signals are no longer blocked\n");
			ssu_check_pending(SIGUSR1, "SIGUSR1"); // SIGUSR1 시그널 유무 확인
		}
	}
	exit(0);
}

void ssu_check_pending(int signo, char *signame) {
	sigset_t sig_set;

	if (sigpending(&sig_set) != 0)
		printf("sigpending() error\n");
	else if (sigismember(&sig_set, signo))
		printf("a %s signal is pending\n", signame);
	else
		printf("%s signals are not pending\n", signame);
}

void ssu_signal_handler(int signo) {
	printf("in ssu_signal_handler fucntion\n");
}
