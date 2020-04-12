#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc < 3) { // 인자 개수가 부족할 경우
		fprintf(stderr, "usage: %s <file1> <file2>\n", argv[0]);
		exit(1);
	}
	if(link(argv[1], argv[2]) == -1) { // argv[1]에 연결된 링크 생성
		fprintf(stderr, "link error for %s\n", argv[1]);
		exit(1);
	}
	exit(0);
}

