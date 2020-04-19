#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	int character;
	while ((character = getc(stdin)) != EOF) // 파일의 끝에 도달하기 전까지 읽음
		if (putc(character, stdout) == EOF) { // 출력
			fprintf(stderr, "standard output error\n");
			exit(1);
		}

		if (ferror(stdin)) { // 에러 존재시 
			fprintf(stderr, "standard input error\n");
			exit(1);
		}

		exit(0);
}
