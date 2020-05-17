#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "usage: %s [Process ID]\n", argv[0]);
		exit(1);
	}
	else
		kill(atoi(argv[1]), SIGKILL); // argv[1]에 존재하는 PID에게 SIGKILL 시그널을 보냄

	exit(0);
}
