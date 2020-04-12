#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt"; // 파일 명
	int count;
	int fd1, fd2;

	if((fd1 = open(fname, O_RDONLY, 0644)) < 0) { // ssu_test.txt라는 이름으로 된 파일을 읽기전용으로 열기
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	fd2 = dup(fd1); // fd2에 fd1의 파일디스크럽터를 복사
	count = read(fd1, buf, 12); // ssu_test.txt의 내용을 12바이트만큼 버퍼에 읽어들임
	buf[count] = 0; // 버퍼의 끝에 널값 지정
	printf("fd1's printf : %s\n", buf);
	lseek(fd1, 1, SEEK_CUR); // fd1의 오프셋을 1만큼 이동
	count = read(fd2, buf, 12); // fd2에 연결된 파일의 내용을 12바이트만큼 버퍼에 읽어들임
	buf[count] = 0; // 끝에 널값 지정
	printf("fd2's printf : %s\n", buf);
	exit(0); // 종료
}
