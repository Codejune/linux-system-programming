#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc != 3) { // 인자 개수가 부족 할 경우
		fprintf(stderr, "usage: %s <oldname> <newname>\n", argv[0]);
		exit(1);
	}

	if(link(argv[1], argv[2]) < 0) { // argv[1]파일에 대한 링크 생성
		fprintf(stderr, "link error\n");
		exit(1);
	}
	else
		printf("step1 passed.\n"); 

	if(remove(argv[1]) < 0) { // 원본 파일 삭제, 참조 카운트가 남아있으므로 디스크에서 지워지지 않음
		fprintf(stderr, "remove error\n");
		remove(argv[2]); // 오류일경우 링크본도 삭제
		exit(1);
	}
	else
		printf("step2 passed.\n");
	printf("Success!\n");
	
	exit(0);
}

