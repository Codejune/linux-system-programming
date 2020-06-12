#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <signal.h> 
#include <syslog.h> 
#include <sys/stat.h> 
#include <sys/types.h>

int ssu_daemon_init(void);

int main(void) {

	printf("daemon proces initialization\n");

	if (ssu_daemon_init() < 0) {
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}
	while (1) {
		printf("daemon process running\n");
		sleep(5);

	}
	exit(0);
}

int ssu_daemon_init(void) { 
	pid_t pid;
	int fd, maxfd;

	if ((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	} else if (pid != 0)
		exit(0);

	pid = getpid();
	printf("process %d running as daemon\n", pid);

	setsid(); // 세션 분리

	signal(SIGTTIN, SIG_IGN); // 터미널 입력 시그널 무시
	signal(SIGTTOU, SIG_IGN); // 터미널 출력 시그널 무시
	signal(SIGTSTP, SIG_IGN); // 터미널 종료 시그널 무시

	maxfd = getdtablesize(); // 디스크럽터 테이블 크기 획득
	for (fd = 0; fd < maxfd; fd++)
		close(fd); // 열린 디스크럽터 모두 닫음

	umask(0); // 마스크 권한 전체 삭제
	chdir("/"); // 루트 디렉토리로 이동
	fd = open("/dev/null", O_RDWR); // 에러 출력 무시
	dup(0); // 디스크럽터 닫음
	dup(0);
	return 0;
}
