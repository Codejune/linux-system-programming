#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

static void ssu_alarm(int signo);
static void ssu_func(int signo);
void ssu_mask(const char *str);

static volatile sig_atomic_t can_jump;
static sigjmp_buf jump_buf;

int main(void)
{
	if (signal(SIGUSR1, ssu_func) == SIG_ERR) { // 시그널 재정의
		fprintf(stderr, "SIGUSR1 error");
		exit(1);
	}

	if (signal(SIGALRM, ssu_alarm) == SIG_ERR) { // 시그널 재정의
		fprintf(stderr, "SIGALRM error");
		exit(1);
	}

	ssu_mask("starting main : ");

	if (sigsetjmp(jump_buf, 1)) { // 분기 설정
		ssu_mask("ending main : ");
		exit(0);
	}

	can_jump = 1;

	while (1)
		pause(); // 대기

	exit(0);
}

void ssu_mask(const char *str) {
	sigset_t sig_set;
	int err_num;

	err_num = errno;

	if (sigprocmask(0, NULL, &sig_set) < 0) {
		printf("sigprocmask() error");
		exit(1);
	}

	printf("%s", str);

	if (sigismember(&sig_set, SIGINT)) // 집합에 시그널 존재 확인
		printf("SIGINT ");

	if (sigismember(&sig_set, SIGQUIT)) // 집합에 시그널 존재 확인
		printf("SIGQUIT ");

	if (sigismember(&sig_set, SIGUSR1)) // 집합에 시그널 존재 확인
		printf("SIGUSR1 ");

	if (sigismember(&sig_set, SIGALRM)) // 집합에 시그널 존재 확인
		printf("SIGARLM ");

	printf("\n");
	errno = err_num;
}

static void ssu_func(int signo) {
	time_t start_time;

	if (can_jump == 0)
		return ;

	ssu_mask("starting ssu_func: ");
	alarm(3);
	start_time = time(NULL);

	while (1)
		if (time(NULL) > start_time + 5) // 5초 지연
			break;

	ssu_mask("ending ssu_func: ");
	can_jump = 0;
	siglongjmp(jump_buf, 1); // 점프
}

static void ssu_alarm(int signo) {
	ssu_mask("in ssu_alarm: ");
}
