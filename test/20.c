#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	char *fname = "ssu_line.txt";
	char *frname = "ssu_blank.txt";
	int fd_w, fd_r;			// 쓰기 파일 디스크럽터, 읽을 파일 디스크럽터
	size_t length;			// read()의 리턴값
	int wordcnt = 1;		// 읽은 파일의 문자의 개수
	char buf[50];			// read()에서 읽는 버퍼
	int i = 0;				// for(), while()문에 조건문 사용

	if ((fd_w = open(fname, O_WRONLY|O_CREAT)) < 0) {
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}
	
	if ((fd_r = open(frname, O_RDONLY|O_CREAT)) < 0) {
		fprintf(stderr, "open error for %s\n", frname);
		exit(1);
	}
	
	dup2(1, 5);
	setbuf(stdout, NULL);
	dup2(fd_w, 1);

	frname = buf;
	while ((length = read(fd_r, frname, 50)) > 0) {
		memset(frname + length, 0, 50 - length);
		frname = strtok_r(frname, " ", &fname);
		printf("%s", frname);
		while((frname = strtok_r(NULL, " ", &fname)) != NULL) {
			printf("\n");
			printf("%s", frname);
			wordcnt++;
		}
		frname = buf;
	}
	dup2(5, 1);
	printf("wordcount = %d\n", wordcnt);
	exit(0);
}
