#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("abort terminate this program\n");
	abort(); // 종료 시그널 호출
	printf("this lie is never reached\n");
	exit(0);
}

