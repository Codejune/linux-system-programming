#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *fname = "ssu_test.txt";
	FILE *fp;

	if((fp = fopen(fname, "r")) == NULL) { // 파일 열기, 읽기 모드
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	else { // 성공적으로 열렸을 경우
		printf("Success!\n");
		printf("Opening \" %s\" in \"r\" mode!\n", fname);
	}
	fclose(fp);
	exit(0);
}
