#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TABLE_SIZE (sizeof(table)/sizeof(*table))

int main(int argc, char *argv[])
{
	struct {
		char *text;
		int mode;
	} table[] = { // 권한 테이블 변수 = 구조체 배열
		{"exists", 0},
		{"execute", 1},
		{"write", 2},
		{"read", 4}
	};
	int i;

	if(argc < 2) { // 입력한 인자의 개수가 부족할 경우
		fprintf(stderr, "usage : %s <file>\n", argv[0]);
		exit(1);
	}

	for(i = 0; i < TABLE_SIZE; i++) {
		if(access(argv[1], table[i].mode) != -1) // 파일 권한 여부 확인(Exist, X, W, R) 
			printf("%s -ok\n", table[i].text);
		else
			printf("%s\n", table[i].text);
	}

	exit(0);
}

