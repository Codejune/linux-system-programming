#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void)
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	switch(sigismember(&set, SIGINT)) // check if sigint signal is member of set
	{
		case 1: // if included
			printf("SIGINT is included. \n");
			break;
		case 0: // if not included
			printf("SIGINT is not included. \n");
			break;
		default: // error
			printf("failed to call sigismember() \n");
	}

	switch(sigismember(&set, SIGSYS)) // check if sigsys signal is member of set
	{
		case 1: // if included
			printf("SIGSYS is included. \n");
			break;
		case 0: // if not included
			printf("SIGSYS is not included. \n");
			break;
		default: // error
			printf("failed to call sigismember() \n");
	}

	exit(0);
}
