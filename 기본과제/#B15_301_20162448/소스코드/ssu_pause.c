#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void ssu_alarm(int signo);

int main(void){

	printf("Alarm Setting\n");
	signal(SIGALRM, ssu_alarm); // SIGALRM에 사용자정의 시그널 등록
	alarm(2); // 2초후 자기 자신에게 SIGALRM을 보냄

	while(1){
		printf("done\n");
		pause(); // SIGALRM시그널이 보내지기 전까지 대기
		alarm(2);
	}

	exit(0);
}

void ssu_alarm(int signo){
	printf("alarm..!!!\n");
}
