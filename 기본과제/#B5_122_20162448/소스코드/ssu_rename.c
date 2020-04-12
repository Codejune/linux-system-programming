#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	if(argc != 3) { // 인자 개수가 부족할 경우
		fprintf(stderr, "usage: %s <oldname> <newname>\n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) { // argv[1] 인자 명으로 읽기전용 열기
		fprintf(stderr, "first open error for %s\n", argv[1]);
		exit(1);
	}
	else
		close(fd); 

	if(rename(argv[1], argv[2]) < 0) { // 파일 이름 변경, 변경할 파일이 이미 존재시 에러
		fprintf(stderr, "rename error\n");
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0) // 다시 argv[1]을 읽기전용 열기
		printf("second open error for %s\n", argv[1]);
	else {
		fprintf(stderr, "it's very strange!\n"); // 변경되었으므로 열기 불가
		exit(1);
	}

	if((fd = open(argv[2], O_RDONLY)) < 0) { // 두번째 인자 파일을 읽기전용으로 열기
		fprintf(stderr, "third open error for %s\n", argv[2]);
		exit(1);
	}

	printf("Everything is good\n");
	exit(0);
}

