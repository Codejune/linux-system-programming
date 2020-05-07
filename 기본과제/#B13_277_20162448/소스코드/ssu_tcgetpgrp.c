#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>

#define STDIN_NUMBER 0

static void ssu_sig_hup(int signum);
static void ssu_print_ids(char *name);

int main(void){
	pid_t pid;
	struct timeval start, end;
	char character;

	gettimeofday(&start, NULL); // 시작 시간 저장

	ssu_print_ids("parent");
	if((pid = fork()) < 0){ // 자식 프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid > 0){ // 부모 프로세스일 경우
		sleep(3);
		exit(0);
	}
	else{ // 자식 프로세스일 경우
		ssu_print_ids("child");
		signal(SIGHUP, ssu_sig_hup); // 시그널 전송
		kill(getpid(), SIGTSTP);
		ssu_print_ids("child");

		if(read(STDIN_FILENO, &character, 1) != 1)
			fprintf(stderr, "read error\n");
		gettimeofday(&end, NULL); // 종료 시간 저장
		printf("%d ms\n", (int)((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec));
		exit(0);
	}
}

static void ssu_sig_hup(int signum){
	printf("ssu_SIG_HUP received, pid = %d\n", getpid());
}

static void ssu_print_ids(char *name){
	printf("[%s] : pid : %d, ppid : %d, pgrp : %d, tpgrp = %d\n",
	name, getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_NUMBER));
	fflush(stdout);
}
