#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

int main(void){
	int fd, state;
	state = seteuid(1000); // EUID를 1000으로 설정

	if(state < 0) { // 변경 실패 시
		fprintf(stderr, "seteuid error\n");
		exit(1);
	}
	if((fd = open("ssu_test.txt", O_CREAT | O_RDWR, S_IRWXU))<0){ //파일 열기, 존재하지 않을 경우 생성
		fprintf(stderr, "open error\n");
		exit(1);
	}
	close(fd);
	exit(0);
}
