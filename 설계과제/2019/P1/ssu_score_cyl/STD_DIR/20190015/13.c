#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
	struct sigaction act;

	signal(SIGALRM, ssu_signal_handler);

	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = ssu_signal_handler;

	sigaction(SIGALRM, &act, NULL);

	alarm(1);

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo)
{
	printf("alarm %d\n", count++);
	alarm(1);

	if(count > 3)
		exit(0);
}
