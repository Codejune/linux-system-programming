#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // fcntl()과 open()을 위한 자료형, 상수, 함수 정의

int main(void) {
	char *fname = "ssu_test.txt"; // open할 파일 이름, 실행 위치 기준 상대경로
	int fd; // 반환된 디스크럽터를 담을 변수

	// ssu_test.txt를 읽기 전용으로 open, 디스크럽터가 0 미만이면 에러 처리
	if((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", fname); // stderr: 에러 출력
		exit(1); // 문제가 있을 시 1, 없으면 0, stdlib.h에 정의되어있음
	} else printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);

	exit(0); // 정상 종료
}
