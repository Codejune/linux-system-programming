#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void)
{
	signal(SIGINT, ssu_signal_handler); // 시그널 재정의

	while (1) {
		if (setjmp(jump_buffer) == 0) { // 분기 설정
			printf("Hit Ctrl-c at anytime ... \n");
			pause();
		}
	}

	exit(0);
}

void ssu_signal_handler(int signo) {
	char character;

	// 시그널 입력 받으면 아래 내용 처리
	signal(signo, SIG_IGN);
	printf("Did you hit Ctrl-c?\n" "Do you really want to quit? [y/n] ");
	character = getchar();

	if (character == 'y' || character == 'Y')
		exit(0);
	else {
		signal(SIGINT, ssu_signal_handler);
		longjmp(jump_buffer, 1);
	}
}
