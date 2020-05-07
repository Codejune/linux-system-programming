#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
	signal(SIGALRM, ssu_signal_handler); // set SIGALRM signal
	alarm(1); // call alarm function

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo){ // Alarm signal handler
	printf("alarm %d\n", count++);
	alarm(1);
}

