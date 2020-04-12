#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	int fd;

	printf("First printf : Hello OSLAB!!\n");

	if((fd = open(fname, O_RDONLY)) < 0) { // 파일 읽기 전용 열기
		fprintF(stderr, "open error for %s\n", fname);
		exit(1);
	}

	if(freopen(fname, "w", stdout) != NULL) // 표준출력을 해당 파일을 열기함과 동시에 출력 연결 변경
		printf("Second printf : Hello, OSLAB!!\n");

	 exit(0);
}
