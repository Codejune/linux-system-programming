#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count =0;

int main(void){
	struct sigaction sig;

	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = ssu_signal_handler;

	sigaction(SIGALRM, &sig, NULL);

	alarm(1);

	while(1);

	exit(0);
}

void ssu_signal_handler(int signo){
	printf("alarm %d\n", count++);

	if(count > 3)
		exit(0);

	alarm(1);
}
