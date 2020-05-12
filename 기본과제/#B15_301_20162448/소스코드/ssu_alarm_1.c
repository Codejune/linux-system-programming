#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
	signal(SIGALRM, ssu_signal_handler); // SIGALRM에 사용자정의 시그널 등록
	alarm(1); // 1초후 자신에게 SIGALRM 시그널을 보냄

	while(1);
	exit(0);
}

void ssu_signal_handler(int signo){ // 시그널 핸들러
	printf("alarm %d\n", count++);
	alarm(1);
}
