#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo)

int count = 0;

int main(void)
{
	struct sigaction sig_act;
	sig_act.sa_handler = ssu_signal_handler;
	sig_act.sa_flags = 0;

	signal(SIGALRM, ssu_signal_handler);
	sigaction(SIGALRM, &sig_act, NULL);
	
	alarm(0);
	while(1) {
		alarm(1);
		sleep(1);
	}

	exit(0);
}

void ssu_signal_handler(int signo) {
	printf("alarm %d\n", count++);
	if(count > 3)
		exit(0);
}
