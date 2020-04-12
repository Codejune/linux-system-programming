#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define MODE_EXEC (S_IXUSR | S_IXGRP | S_IXOTH)

int main(int argc, char *argv[])
{
	struct stat statbuf;
	int i;

	if(argc < 2) { // 입력한 인자의 개수가 부족할 경우
		fprintf(stderr, "usage : %s <file1> <file2> ... <fileN>\n", argv[0]);
		exit(1);
	}

	for(i = 1; i < argc; i++) {
		if(stat(argv[i], &statbuf) < 0)  { // 인자로 들어온 파일의 정보를 가져옴 
			fprintf(stderr, "%s : stat error\n", argv[i]);
			continue;
		}
		statbuf.st_mode |= MODE_EXEC; // 유저, 그룹, 다른사람의 실행권한을 추가
		statbuf.st_mode ^= (S_IXGRP | S_IXOTH); // 그룹과 다른사람의 실행권한을 토글

		if(chmod(argv[i], statbuf.st_mode) < 0) // 권한 지정
			fprintf(stderr, "%s : chmod error\n", argv[i]);
		else
			printf("%s : file permission was changed.\n", argv[i]);
	}

	exit(0);
}

