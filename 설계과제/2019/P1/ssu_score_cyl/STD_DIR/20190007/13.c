#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
	struct sigaction sigact;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = ssu_signal_handler;
	sigaction(SIGALRM, &sigact,NULL);

	alarm(1);

	while(1){
		pause();
		alarm(1);
	}

	exit(1);
}

void ssu_signal_handler(int signo) {
	printf("alarm %d\n", count++);

	if(count > 3)
		exit(0);
}

