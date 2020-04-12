#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	struct stat statbuf; // sys/stat.h 에 정의되어있는 stat구조체 선언
	if (argc != 2) { // 인자의 개수가 부족할 경우
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}

	if((stat(argv[1], &statbuf)) < 0) { // 인자로 받은 파일명의 정보를 statbuf 변수에 담음
		fprintf(stderr, "stat error\n");
		exit(1);
	}

	printf("%s is %ld bytes\n", argv[1], statbuf.st_size);
	exit(0); // 정상 종료
}

