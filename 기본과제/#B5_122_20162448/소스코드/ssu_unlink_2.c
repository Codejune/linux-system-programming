#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char buf[64];
	char *fname = "ssu_tempfile";
	int fd;
	int length;
	if((fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0600)) < 0) { // ssu_tempfile을 읽기쓰기 모드로 없으면 생성하고, 있으면 초기화 후 다시 씀
		fprintf(stderr, "first open error for %s\n", fname);
		exit(1);
	}

	if(unlink(fname) < 0) { // 연결 끊음, 그러나 레퍼런스 카운트 > 0 
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}

	if(write(fd, "How are you?", 12) != 12) { // 연결을 끊은 해당 파일에 텍스트 쓰기
		fprintf(stderr, "write error\n");
		exit(1);
	}

	lseek(fd, 0, 0); // 오프셋 처음으로 이동
	if((length = read(fd, buf, sizeof(buf))) < 0) { // 파일의 내용을 읽어들임
		fprintf(stderr, "buf read error\n");
		exit(1);
	}
	else
		buf[length] = 0;
	printf("%s\n", buf);
	close(fd); // 레퍼런스 카운트 감소

	if((fd = open(fname, O_RDWR)) < 0) { // 다시 열었을 때는 레퍼런스 카운트가 0이므로 열수 없음
		fprintf(stderr, "second open error for %s\n", fname);
		exit(1);
	}
	else
		close(fd);
	exit(0);
}

