#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main(void)
{

	printf("this is the original program\n");
	execl("./ssu_execl_test_1", "ssu_execl_test_1", "param1", "param2", "param3", (char *)0); // 인자들 전달 및 실행, 순서대로 0, 1, 2 ...
	printf("%s\n", "this line should never get printed\n");
	exit(0);
}
