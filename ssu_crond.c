/*
 * @file ssu_crond.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crond.h"

#define DEBUG

char pwd[MAX_BUFFER_SIZE];
/**
 * @brief ssu_crond 메인 함수
 */
int main(void)
{
	char command[MAX_BUFFER_SIZE];

	getcwd(pwd, MAX_BUFFER_SIZE);
#ifdef DEBUG
	printf("main(): pwd = %s\n", pwd);
#endif
	set_daemon_process();
	sprintf(command, "%s/test.sh");
	exit(0);
}

/**
 * @brief 데몬 프로세스 설정
 */
void set_daemon_process(void) // 데몬 프로세스 설정
{
	pid_t pid;
	int fd, maxfd;

	// #1 프로세스 분리 
	if((pid = fork()) < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	} else if(pid != 0) 
		exit(0);

	// #2 새로운 프로세스 그룹 생성
	setsid(); 

	// #3 터미널 입출력 시그널 무시
	signal(SIGTTIN, SIG_IGN); // STDIN 무시
	signal(SIGTTOU, SIG_IGN); // STDOUT 무시
	signal(SIGTSTP, SIG_IGN); // STDERR 무시

	// #4 파일 모드 생성 마스크 해제
	umask(false);

	// #5 루트 디렉토리 이동
	chdir("/");

#ifdef DEBUG
	/*
#endif
	// #6 모든 파일 디스크럽터 연결 종료
	maxfd = getdtablesize(); // 모든 파일 디스크럽터 개수 획득
	for(fd = 0; fd < maxfd; fd++) 
		close(fd);

	// #7 표준 입출력 및 에러 재지정
	fd = open("dev/null", O_RDWR); // STDIO 재설정
	dup(0);
	dup(0);
#ifdef DEBUG
	*/
#endif
}

