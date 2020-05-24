#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void)
{
	int testfd;
	int fd;

	// 파일 열기
	fd  = open("test.txt", O_CREAT);
	// 파일 디스크럽터 3을 5로 복사
	testfd = fcntl(fd, F_DUPFD, 5);
	printf("testfd : %d\n", testfd);
	// 파일 디스크럽터 5을 6으로 복사
	testfd = fcntl(fd, F_DUPFD, 6);
	printf("testfd : %d\n", testfd);

	getchar();
}
