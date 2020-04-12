#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

struct stat statbuf; // sys/stat.h에 정의되어있는 stat구조체 

void ssu_checkfile(char *fname, time_t *time);

int main(int argc, char *argv[])
{
	time_t intertime; // sys/stat.h에 정의되어있는 time_t

	if(argc != 2) { // 입력한 인자의 개수가 부족할 경우
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}

	if(stat(argv[1], &statbuf) < 0) { // statbuf변수에 인자로 입력한 파일의 정보를 저장
		fprintf(stderr, "stat error for %s\n", argv[1]);
		exit(1);
	}

	intertime = statbuf.st_mtime; // 파일의 수정 시간 정보를 저장
	while(1) {
		ssu_checkfile(argv[1], &intertime);
		sleep(10); // 0.01초 대기
	}
}

void ssu_checkfile(char *fname, time_t *time) {
	if(stat(fname, &statbuf) < 0) { // statbuf변수에 넘겨받은 파일이름의 정보를 저장
		fprintf(stderr, "Warning : ssu_checkfile() error!\n");
		exit(1);
	}
	else
		if(statbuf.st_mtime != *time) { // 파일의 수정 시간 정보가 다를 경우 (수정됨)
			printf("Warning : %s was modified!.\n", fname);
			*time = statbuf.st_mtime;
		}
}
			

