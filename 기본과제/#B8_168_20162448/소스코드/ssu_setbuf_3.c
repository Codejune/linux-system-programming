#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(void)
{
	char buf[BUFFER_SIZE];
	int a, b;
	int i;

	setbuf(stdin, buf); // 버퍼 설정
	scanf("%d %d", &a, &b);
	for (i= 0; buf[i] != '\n'; i++) // 개행 문자가 나오기 전까지 증가
		putchar(buf[i]); // 화면에 출력

	putchar('\n');
	exit(0);
}
