#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	sigset_t pendingset;
	sigset_t sig_set;
	int count =0;

	sigfillset(&sig_set); // make all bit 1
	sigprocmask(SIG_SETMASK, &sig_set, NULL); // set sig_set to mask

	while(1){
		printf("count: %d\n", count++);
		sleep(1);

		if(sigpending(&pendingset) == 0){ // if signal is pending
			if(sigismember(&pendingset, SIGINT)){ // if pended signal is SIGINT
				printf("SIGINT가 블록되어 대기중. 무한루프를 종료.\n");
				break;
			}
		}
	}
	exit(0);
}
