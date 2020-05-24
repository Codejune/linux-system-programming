#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	int fd;
	int val;
	// 파일 생성 및 열기
	if ((fd = open("exec_copy.txt", O_CREAT))<0)
	{
		fprintf(stderr, "open error for %s\n", "exec_copy.txt");
		exit(1);
	}
	// 플래그 비트 획득
	val = fcntl(fd, F_GETFD, 0);
	// 플래그에서 켜져있을 경우
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	// 플래그에서 꺼져있을 경우
	else
		printf("close-on-exec bit off\n");
	// 비트 켜기
	val|=FD_CLOEXEC;
	// 비트 확인
	if(val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");

	// 플래그 비트 획득
	fcntl(fd, F_SETFD, val);
	// 빈 루프 실행
	execl("./loop", "./loop", NULL);
}
