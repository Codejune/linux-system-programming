#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt"; // 파일 명
	int fd;
	int length;

	if((fd = open(fname, O_RDONLY, 0644)) < 0) { // ssu_test.txt를 읽기전용으로 열기
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	if(dup2(1, 4) != 4) { // 표준 입력 디스크럽터를 4번 파일디스크럽터로 복사
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}

	while(1) {
		length = read(fd, buf, BUFFER_SIZE); // ssu_test.txt파일을 1024바이트만큼 버퍼에 읽어들임
		if(length <= 0) 
			break;

		write(4, buf, length); // fd[4]에 버퍼에있는 내용을 씀 == 표준출력
	}
	exit(0);
}
	
