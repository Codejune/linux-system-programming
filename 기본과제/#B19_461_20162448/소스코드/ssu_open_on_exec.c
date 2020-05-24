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
	fd = open("exec_copy.txt", O_CREAT);
	// 빈 루프 실행
	execl("./loop", "./loop", NULL);
	exit(0);
}
