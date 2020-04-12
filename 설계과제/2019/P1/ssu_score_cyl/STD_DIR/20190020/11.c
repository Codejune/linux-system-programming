#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

void ssu_signal(int signo){
	printf("SIGUSR1 catched!!\n");
}

int main(void)
{
	pid_t pid;
	sigset_t sigset, pending_sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR1);
	sigprocmask(SIG_BLOCK,&sigset,NULL);

	signal(SIGUSR1,ssu_signal);
	kill(getpid(),SIGUSR1);

	if((pid=fork())<0){
		fprintf(stderr,"fork error\n");
		exit(1);
	}
	else if(pid){
		sigpending(&pending_sigset);
		if(sigismember(&pending_sigset,SIGUSR1)){
			printf("child : SIGUSR1 pending\n");
		}
	}else{
		sigpending(&pending_sigset);
		if(sigismember(&pending_sigset,SIGUSR1)){
			printf("parent : SIGUSR1 pending\n");
		}
	}
}

