#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	sigset_t sig_set;
	int count;

	sigemptyset(&sig_set); // initialize signal set
	sigaddset(&sig_set, SIGINT); // add SIGINT into set
	sigprocmask(SIG_BLOCK, &sig_set, NULL); // block

	for(count = 3; 0 < count; count --){
		printf("count %d\n", count);
		sleep(1);
	}

	printf("Ctrl-C에 대한 블록을 해제\n"); 
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL);//unblock
	printf("count중 Ctrl-C입력하면 이 문장은 출력되지 않음.\n");

	while(1);

	exit(0);
}
