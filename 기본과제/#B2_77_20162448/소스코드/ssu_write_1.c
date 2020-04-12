#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE]; // 버퍼
	int length; 

	length = read(0, buf, BUFFER_SIZE); // 표준 입력에서 입력받은 문자들을 버퍼에 1024바이트만큼 받음
	write(1, buf, length); // 표준 출력에 버퍼를 출력
	exit(0);
}
