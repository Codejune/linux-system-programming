#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#define MSG "message will be written to terminal\n"

int main(void)
{
	int new_fd;
	// 디스크럽터 1을 3으로 복사
	if ((new_fd = fcntl(STDOUT_FILENO, F_DUPFD, 3))==-1)
	{
		fprintf(stderr, "Error : Copying File Descriptor\n");
		exit(1);
	}
	// 디스크럽터 1(표준출력) 닫음
	close(STDOUT_FILENO);
	// 디스크럽터 3에 문자열을 씀
	write(3, MSG, strlen(MSG));
	exit(0);
}
