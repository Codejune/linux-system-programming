#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void ssu_signal_handler1(int signo);
void ssu_signal_handler2(int signo);

int main(void)
{
	if(signal(SIGINT, ssu_signal_handler1) == SIG_ERR){ // SIGINT에 사용자정의 시그널 등록
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGUSR1, ssu_signal_handler2) == SIG_ERR){ // 사용자정의 시그널에 내용 등록
		fprintf(stderr, "cannot handle SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	raise(SIGINT); // 자기 자신에게 SIGINT 시그널을 보냄
	raise(SIGUSR1); // 자기 자신에게 SIGUSR1 시그널을 보냄
	printf("main return\n");

}

void ssu_signal_handler1(int signo){
	printf("SIGINT signal !\n");
}

void ssu_signal_handler2(int signo){ 
	printf("SIGUSR1 signal !\n");
}
