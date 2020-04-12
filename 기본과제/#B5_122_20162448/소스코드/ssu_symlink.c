#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc != 3) { // 인자의 개수가 부족할 경우
		fprintf(stderr, "usage:%s <actualname> <symname>\n", argv[0]);
		exit(1);
	}

	if(symlink(argv[1], argv[2]) < 0) { // argv[1]의 파일로 연결된 심링크 argv[2] 파일
		fprintf(stderr, "symlink error\n");
		exit(1);
	}
	else
		printf("symlink: %s -> %s\n", argv[2], argv[1]); 
	exit(0);
}

