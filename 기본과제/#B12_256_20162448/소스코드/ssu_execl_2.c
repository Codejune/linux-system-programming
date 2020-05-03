#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	if(fork() == 0){ //자식 프로세스 생성
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);
		fprintf(stderr, "exec error\n");//수행 프로세스가 원래 프로세스 덮어써서 출력 안됨
		exit(1);//출력X
	}

	if(fork() == 0){ //자식프로세스 생성
		execl("/bin/echo", "echo", "this is", "message two", (char *)0);
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	if(fork() == 0){ //자식 프로세스 생성
		execl("/bin/echo", "echo", "this is", "message three", (char *)0);
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	printf("Parent program ending\n"); 
	exit(0);
}

