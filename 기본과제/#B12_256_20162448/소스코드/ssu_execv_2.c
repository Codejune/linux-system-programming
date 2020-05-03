#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char *argv[] ={
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};//argument 배열을 만들어놓고 사용

	printf("this is the original program\n");
	execv("./ssu_execl_test_1", argv); //새 프로세스 시작
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
