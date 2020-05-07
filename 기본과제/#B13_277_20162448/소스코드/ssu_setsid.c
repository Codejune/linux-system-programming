#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int main(void){
	pid_t pid;

	if((pid = fork()) < 0){ // 자식 프로세스 생성
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	else if(pid != 0) // 부모 프로세스 종료
		exit(1);
	printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	setsid();
	printf("after pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	exit(0);
}
