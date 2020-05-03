#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char *argv[] = { // 인자 배열
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};

	char *env[] = { // 환경변수 인자 배열
		"NAME = value",
		"nextname=nextvalue",
		"HOME=/home/oslab",
		(char *)0
	};

	printf("this is the original program\n");
	execve("./ssu_execl_test_1", argv,env); // 프로세스 시작 및 자기자신 종료, 인자 전달
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
