#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_MAX 4

int main(int argc, char *argv[])
{
	int fd;					// 읽을 파일 디스크럽터
	size_t n;				// read()의 리턴값
	int count = 0;			// 읽은 파일의 라인 수
	char buf[2], cbuf[5];	// read()에서 읽는 버퍼와 출력할 버퍼

	if (argc < 1) {
		fprintf(stderr, "usage : %s file\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	sprintf(cbuf, "%d  ", count);
	while ((n = read(fd, buf, 1)) > 0) {

		if (strlen(cbuf) >= BUF_MAX) {
			write(1, cbuf, BUF_MAX);
			memset(cbuf, 0, BUF_MAX);
		}

		strncpy(cbuf + strlen(cbuf), buf, 1);

		if (buf[0] == '\n') {
			write(1, cbuf, strlen(cbuf)); // 표준 출력에 출력
			sprintf(cbuf, "%d  ", ++count);
		}
	}
		
	close(fd);
}
