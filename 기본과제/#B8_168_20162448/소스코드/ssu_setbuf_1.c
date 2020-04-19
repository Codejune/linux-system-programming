#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
int main(void)
{
	char buf[BUFFER_SIZE];
	setbuf(stdout, buf); // 버퍼 설정
	printf("Hello, ");
	sleep(1); // 대기
	printf("OSLAB!!");
	sleep(1);
	printf("\n");
	sleep(1); // 버퍼에 넣은 뒤 한번에 출력

	setbuf(stdout, NULL); // 버퍼가 존재하지 않음
	printf("How"); // 즉시 출력
	sleep(1);
	printf(" are");
	sleep(1);
	printf(" you?");
	sleep(1);
	printf("\n");
	exit(0);
}
