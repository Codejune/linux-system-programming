#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void ssu_alarm(int signo);

int main(void){

	printf("Alarm Setting\n");
	signal(SIGALRM, ssu_alarm); // set SIGALRM signal
	alarm(2); // wait until 2 seconds

	while(1){
		printf("done\n");
		pause(); // wait until catch alarm signal
		alarm(2);
	}

	exit(0);
}

void ssu_alarm(int signo){
	printf("alarm..!!!\n");
}
