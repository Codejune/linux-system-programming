#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	FILE *fp;
	int character;
	if (argc!= 2) { // 인자 개수 부족 시
		fprintf(stderr, "usage: %s <filename>\n", argv[0])
		exit(1);
	}

	if ((fp= fopen(argv[1], "r")) == NULL) { // 읽기 모드 열기
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}

	character = fgetc(fp); // 문자 하나 읽어들임
	while (!feof(fp)) { // 파일의 끝이 나오기 전까지
		fputc(character, stdout); // 표준 출력
		if (ferror(fp)) { // 에러시
			fprintf(stderr, "Error detected!!\n");
			clearerr(fp);
		}
		character = fgetc(fp); // 다시 읽어들임
	}

	fclose(fp); // 파일 닫음
	exit(0);
}
