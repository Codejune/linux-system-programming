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
	pid_t pid;

	pid = getpid();
	printf("parent process : %d\n", pid);
	printf("daemot process initializations\n");

	if(ssu_daemon_init() < 0){ // 디몬 프로세스 생성
		fprintf(stderr, "ssu_daemon_init failed\n");
		exit(1);
	}
	exit(0);
}

int ssu_daemon_init(void){
	pid_t pid;
	int fd, maxfd;

	if((pid = fork()) < 0){ // 자식 프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid != 0) // 부모 프로세스 종료
		exit(0);

	pid = getpid(); // 자식 프로세스 ID 획득
	printf("process %d running as daemon\n", pid);
	setsid(); // 새로운 세션 생성
	/*STD I/O 무시*/
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	maxfd = getdtablesize(); // 파일 디스크럽터 최대 개수 획득

	//파일 디스크럽터 전부 연결 끊음
	for(fd = 0;fd < maxfd;fd++)
		close(fd);

	umask(0);
	chdir("/"); // 루트로 이동
	fd = open("/dev/null", O_RDWR);
	dup(0); // 디스크럽터 STDOUT으로 연결
	dup(0);
	while(1){
		sleep(3);
	}
}
