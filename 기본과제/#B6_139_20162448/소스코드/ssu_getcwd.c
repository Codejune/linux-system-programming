#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 1024

int main(void)
{
	char *pathname;

	if(chdir("/home/codejune")<0){ // 현재 디렉토리를 /home/codejune 변경
		fprintf(stderr,"chdir error\n");
		exit(1);
	}

	pathname = malloc(PATH_MAX); // 메모리 할당

	if(getcwd(pathname,PATH_MAX)==NULL){ // pathname에 현재 위치에 대한 절대경로를 저장
		fprintf(stderr,"getcwd error\n");
		exit(1);
	}

	printf("current directory = %s\n",pathname);
	exit(0);

}
