#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // 표준 심볼 상수 및 자료형, 여기에선 close()를 위해 사용
#include <fcntl.h> // open(), fcntl() 등을 위해 사용
int main(void) 
{
	char *fname = "ssu_test.txt"; // open할 파일 이름, 실행 위치 기준 상대경로
	int fd; // 반환된 디스크럽터를 담을 변수

	if((fd = creat(fname, 0666)) < 0) { // 유저, 그룹, 다른사용자까지 모두 읽고 쓸 수 있게 파일 생성
		// 0666은 기본 파일 생성 권한과 동일
		fprintf(stderr, "create error for %s\n", fname); // stderr: 에러 출력 
		exit(1); // 문제가 있을 시 1, 없으면 0, stdlib.h에 정의되어 있음
	} 
	else {
		close(fd); // 정상적으로 생성되면 디스크럽터의 연결을 끊고
		fd = open(fname, O_RDWR); // ssu_test.txt를 읽기/쓰기 모드로 open하여 해당 디스크럽터를 받음
		printf("Succeeded!\n<%s> is new readable and writeable\n", fname);
	}

	exit(0); // 정상 종료
}
