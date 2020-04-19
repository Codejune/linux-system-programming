#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);
int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname= "/dev/pts/19"; // 파일 명
	FILE *fp;

	if ((fp= fopen(fname, "w")) == NULL) { // 쓰기 전용 모드로 파일 열기
		fprintf(stderr, "fopen error for %s", fname);
		exit(1);
	}

	ssu_setbuf(fp, buf);
	fprintf(fp, "Hello, ");
	sleep(1);
	fprintf(fp, "UNIX!!");
	sleep(1);
	fprintf(fp, "\n");
	sleep(1); // 버퍼에 넣은 후 한번에 출력
	ssu_setbuf(fp, NULL);
	fprintf(fp, "HOW");
	sleep(1);
	fprintf(fp, " ARE");
	sleep(1);
	fprintf(fp, " YOU?");
	sleep(1);
	fprintf(fp, "\n");
	sleep(1); // 버퍼에 넣지 않고 바로 출력
	exit(0);
}

void ssu_setbuf(FILE *fp, char *buf)
{
	size_tsize;
	int fd;
	int mode;

	fd= fileno(fp);
	if (isatty(fd)) // 터미널번호 확인
		mode = _IOLBF; // 개행문자 사용, 버퍼가 가득 차면 버퍼 삭제
	else
		mode = _IOFBF; // 버퍼를 입출력에 사용,
	if (buf== NULL) {
		mode = _IONBF; // 사용된 버퍼가 없음
		size = 0;
	}
	else
		size = BUFFER_SIZE;

	setvbuf(fp, buf, mode, size);
}
