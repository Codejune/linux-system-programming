#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	char *mode = "r"; // 권한

	if(fopen(fname, mode) == NULL) { // 파일 열기, 읽기모드
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	else
		printf("Success!\nFilename: <%s>, mode: <%s>\n", fname, mode);

	exit(0);
}
