#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// 라인 개수 체크 함수
int main(void)
{
	char character;
	int fd;
	int line_count = 0;

	if((fd = open("ssu_test.txt", O_RDONLY)) < 0 ) { // ssu_test.txt파일을 읽기전용으로 열기
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}

	while(1) {
		if(read(fd, &character, 1) > 0) { // 한바이트를 character 변수에 읽어서 저장
			if(character == '\n') 
				line_count++; 
		}
		else
			break;
	}

	printf("Total line : %d\n", line_count);
	exit(0);
}
