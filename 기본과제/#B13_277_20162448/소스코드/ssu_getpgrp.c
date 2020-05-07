#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main(void){
	pid_t pgid;
	pid_t pid;

	pid = getpid(); // PID 획득
	pgid = getpgrp(); // GPID 획득
	printf("pid : %d, pgid: %d\n", pid, pgid);

	exit(0);
}
