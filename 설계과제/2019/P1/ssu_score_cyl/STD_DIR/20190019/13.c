#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count =0;

int main(void)
{
	sigaction(SIGALRM,(void *)ssu_signal_handler,NULL);

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo) {
	printf("alarm %d\n", count++);
	sleep(1);

	if (count >3)
		exit(0);
}
