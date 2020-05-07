#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main(void){
	pid_t pgid;
	pid_t pid;
	struct timeval start, end;
	gettimeofday(&start, NULL); // 시작 시간 저장

	pid = getpid(); // PID 획득
	pgid = getpgrp(); // GPID 획득
	printf("pid : %d, pgid: %d\n", pid, pgid);

	gettimeofday(&end, NULL); // 종료 시간 저장
	printf("%d ms\n", (int)((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
	exit(0);
}
