#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	sigset_t sig_set;
	int count;

	sigemptyset(&sig_set); // sig_set 초기화
	sigaddset(&sig_set, SIGINT); // sig_set에 SIGINT 시그널 추가
	sigprocmask(SIG_BLOCK, &sig_set, NULL); // 블록

	for(count = 3; 0 < count; count --){
		printf("count %d\n", count);
		sleep(1);
	}

	printf("Ctrl-C에 대한 블록을 해제\n");
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL); // 블록 해제
	printf("count중 Ctrl-C입력하면 이 문장은 출력되지 않음.\n");

	while(1);

	exit(0);
}
