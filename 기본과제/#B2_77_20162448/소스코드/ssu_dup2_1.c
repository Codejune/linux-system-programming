#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char *fname = "ssu_test.txt"; // 파일 명
	int fd;

	if((fd = creat(fname, 0644)) < 0) { // ssu_test.txt라는 이름으로 권한 0644를 줘서 파일 생성
		printf("creat error for %s\n", fname);
		exit(1);
	}
	
	printf("First printf is on the screen.\n"); // 화면에 출력
	dup2(fd, 1); // 현재 디스크럽터를 표준 입력으로 연결
	printf("Second printf is in this file.\n"); // 파일에 쓰기 
	exit(0); // 종료
}
