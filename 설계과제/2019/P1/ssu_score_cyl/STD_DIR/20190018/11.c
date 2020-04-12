#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void ssu_signal(int signo){
	printf("SIGUSR1 catched!!\n");
}

int main(void)
{
	pid_t pid;
	sigset_t sigset;
	sigset_t pending_sigset;

	signal(SIGUSR1, ssu_signal);

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);
	kill(getpid(), SIGUSR1);


	if((pid = fork()) < 0){
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid == 0){
		if(sigpending(&pending_sigset) == 0){
			if(sigismember(&pending_sigset, SIGUSR1)){
				printf("child : SIGUSR1 pending\n");
			}

			exit(0);
		}
	}
	else{
		if(sigpending(&pending_sigset) == 0){
			if(sigismember(&pending_sigset, SIGUSR1)){
				printf("parent : SIGUSR1 pending\n");
			}

			exit(0);
		}
	}

}
