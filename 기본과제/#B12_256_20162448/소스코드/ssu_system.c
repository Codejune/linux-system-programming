#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
	printf("before system()\n");
	system("pwd");//시스템 명령인 pwd를 실행
	printf("after system()\n");
	exit(0);
}
