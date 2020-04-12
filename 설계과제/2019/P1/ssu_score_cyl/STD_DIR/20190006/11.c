#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void ssu_signal(int signo){
	printf("SIGUSR1 catched!!\n");
}

int main()
{
	pid_t pid;
	sigset_t sigset;
	sigset_t pending_set;

	signal(SIGUSR1, ssu_signal);
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR1);
	sigprocmask(SIG_BLOCK,&sigset,NULL);

	kill(getpid(),SIGUSR1);

	if((pid=fork())<0){
		fprintf(stderr,"fork err\n");
		exit(1);
	}
	else if(pid==0){
		sigpending(&pending_set);
		if(sigismember(&pending_set,SIGUSR1)){
			printf("child : SIGUSR1 pending\n");
			exit(0);
		}
	}
	else {
		sigpending(&pending_set);
		if(sigismember(&pending_set,SIGUSR1)){
			printf("parent : SIGUSR1 pending\n");
			exit(0);
		}
	}
}
