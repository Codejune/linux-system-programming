#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	sigset_t pendingset;
	sigset_t sig_set;
	int count =0;

	sigfillset(&sig_set); // 모든 비트를 1로 만듦
	sigprocmask(SIG_SETMASK, &sig_set, NULL); // 마스킹

	while(1){
		printf("count: %d\n", count++);
		sleep(1);

		if(sigpending(&pendingset) == 0){ // 시그널이 pending되면
			if(sigismember(&pendingset, SIGINT)){ // pended된 시그널이 SIGINT일 때
				printf("SIGINT가 블록되어 대기중. 무한루프를 종료.\n");
				break;
			}
		}
	}
	exit(0);
}
