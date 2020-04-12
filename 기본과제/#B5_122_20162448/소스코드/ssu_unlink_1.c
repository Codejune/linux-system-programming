#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char *fname = "ssu_dump.txt"; // 파일 이름
	if(open(fname, O_RDWR) < 0) { // ssu_dump.txt를 읽기 쓰기 모드로 열기
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	
	if(unlink(fname) < 0) { // 해당 파일의 연결을 끊음== rm
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}
	printf("File unlinked\n");
	sleep(20);
	printf("Done\n");
	exit(0);
}

