#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char c;
	int fd;

	if((fd = open("ssu_test.txt", O_RDONLY)) < 0) { // ssu_test.txt를 읽기전용으로 열기
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}

	while(1) { // 한바이트씩 읽어들여서 화면에 출력
		if(read(fd, &c, 1) > 0 )
			putchar(c);
		else
			break;
	}
	exit(1);
}
