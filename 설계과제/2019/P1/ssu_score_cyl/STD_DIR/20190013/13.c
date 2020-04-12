#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;

int main(void)
{
	struct sigaction sig_action;

	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags = 0;
	sig_action.sa_handler = ssu_signal_handler;
	sigaction(SIGALRM, &sig_action, NULL);

	alarm(1);

	while (1);

	exit(0);
}

void ssu_signal_handler(int signo) {
	printf("alarm %d\n", count++);

	if (count > 3)
		exit(0);

	alarm(1);
}
