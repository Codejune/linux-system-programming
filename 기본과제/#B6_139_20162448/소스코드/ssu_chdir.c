#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	if(chdir("/etc")<0){ // 현재 디렉토리 경로를 /etc/.로 이동
		fprintf(stderr,"chdir error\n");
		exit(1);
	}

	printf("chdir to /etc succeeded.\n");
	exit(0);
}
