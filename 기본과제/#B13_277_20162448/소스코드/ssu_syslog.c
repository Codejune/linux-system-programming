#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);

int main(void)
{

	printf("daemon process initialization\n");

	if(ssu_daemon_init() < 0) { // 디폰 프로세스 생성
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}
	while(1) {
		openlog("lpd", LOG_PID, LOG_LPR); // 로그 파일 열기
		syslog(LOG_ERR, "open failed lpd %m"); // 로그 작성
		closelog(); // 로그 파일 닫음
		sleep(5);
	}
	exit(0);
}

int ssu_daemon_init(void) {
	pid_t pid;
	int fd, maxfd;
	if((pid = fork()) < 0) { // 자식 프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if (pid != 0) // 부모 프로세스 종료
		exit(0);
	pid = getpid(); // 자식 프로세스 PID 획득
	printf("process %d running as daemon\n", pid);
	setsid(); // make a new session
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize(); // 파일 디스크럽터 최대 개수 획득
	for(fd = 0; fd < maxfd; fd++)
		close(fd);
	umask(0); // UMASK 지정
	chdir("/"); // 루트 디렉토리 이동
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	return 0;
}
