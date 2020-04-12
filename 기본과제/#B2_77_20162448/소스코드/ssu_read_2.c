#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	fd1 = open(fname, O_RDONLY, 0644); // 첫번째 연결(3)
	fd2 = open(fname, O_RDONLY, 0644); // 두번째 연결(4)

	if(fd1 < 0 || fd2 < 0) { // 디스크럽터가 제대로 연결이 되어있지 않을 경우 오류 처리
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	count = read(fd1, buf, 25); // 첫번째 연결에서 25바이트 만큼 버퍼에 읽어들임, count = 25
	buf[count] = 0; // 끝 지점을 0을 주어 문장의 끝을 알림 
	printf("fd1's first printf : %s\n", buf);
	lseek(fd1, 1, SEEK_CUR); // 오프셋 현재위치에서 1만큼 이동 
	count = read(fd1, buf, 24); // 다시 24바이트를 읽어들임
	buf[count] = 0; // 끝지점 지정
	printf("fd1's second printf : %s\n", buf);
	count = read(fd2, buf, 25); // 두번째 연결에서 25바이트 만큼 버퍼에 읽어들임, count = 25
	buf[count] = 0; // 끝지점 지정
	printf("fd2's first printf : %s\n", buf);
	lseek(fd2, 1, SEEK_CUR); // 오프셋 현재위치에서 1만큼 이동
	count = read(fd2, buf, 24); // 다시 24바이트를 읽어들임
	buf[count] = 0; // 끝지점 지정
	printf("fd2's second printf : %s\n", buf);

	exit(0);
}
