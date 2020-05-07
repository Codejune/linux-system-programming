#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main(int argc, char *argv[]){
	pid_t pgid;
	pid_t pid;

	if(argc < 2){
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}

	pid = getpid(); // PID 획득
	pgid = getpgid(atoi(argv[1])); // argv[1]에 해당하는 프로세스가 속한 그룹의 id를 갖고온다
	printf("pid: %d, pgid: %d\n", pid, pgid);

	exit(0);
}
