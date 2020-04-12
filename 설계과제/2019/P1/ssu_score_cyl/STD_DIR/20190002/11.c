#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal(int signo)
{
	printf("SIGUSR1 catched!!\n");
}

int main(void)
{
	pid_t pid;
	sigset_t sigset;
	sigset_t pending_sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	signal(SIGUSR1, ssu_signal);
	kill(getpid(), SIGUSR1);

	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	else if (pid == 0)
	{
		sigpending(&sigset);

		switch (sigismember(&sigset, SIGUSR1))
		{
			case 1:
				printf("parent : SIGUSR1 pending\n");
				break;

			case 0:
				break;
		}
	}

	else
	{
		sigpending(&sigset);

		switch (sigismember(&sigset, SIGUSR1))
		{
			case 1:
				printf("child : SIGUSR1 pending\n");
				break;

			case 0:
				break;
		}
	}

	exit(0);
}
