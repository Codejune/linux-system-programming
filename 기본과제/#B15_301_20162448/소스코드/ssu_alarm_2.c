#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define LINE_MAX 2048

static void ssu_alarm(int signo);

int main(void)
{
	char buf[LINE_MAX];
	int n;


	if(signal(SIGALRM, ssu_alarm) == SIG_ERR){ // SIGALRM에 사용자정의 시그널 등록
		fprintf(stderr, "SIGALRM error\n");
		exit(1);
	}

	alarm(10); // 10초후 자신에게 SIGALRM 시그널을 보냄

	if((n = read(STDIN_FILENO, buf, LINE_MAX)) < 0){ // 표준입력으로부터 문자열을 읽음
		fprintf(stderr, "read() error\n");
		exit(1);
	}

	alarm(0);
	write(STDOUT_FILENO, buf, n); // 터미널에 입력받은 내용을 출력
	exit(0);
}

static void ssu_alarm(int signo){
	printf("ssu_alarm() called!\n");
}
