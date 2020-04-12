#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count;

int main(void)
{
	struct sigacion sig_act;
	
	signal(SIGALRM, ssu_signal_handler);

	sig_act.sa_flags = 0;
	sig_act.sa_handler = ssu_signal_handler;
	sigprocmask(SIG_SETMASK, sig_act.sa_mask, NULL);
	
	sigaction(SIGALRM, &sig_act, NULL);

	while (1);

	exit(0);
}

void ssu_signal_handler(int signo)
{
	printf("alarm %d\n", count++);

	if (count > 3) exit(0);
}
