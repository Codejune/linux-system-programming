#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // 표준 심볼 상수 및 자료형, 여기에선 lseek()을 위해 사용
#include <fcntl.h> // open(), fcntl()등을 위해 사용
#include <sys/stat.h>

// CREAT_MODE 상수 선언, 0644와 동일
#define CREAT_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char buf1[] = "1234567890"; // 버퍼 1
char buf2[] = "ABCDEFGHIJ"; // 버퍼 2

int main(void) 
{
	char *fname = "ssu_hole.txt"; // 생성 및 쓰기 할 파일 이름, 실행 위치 기준 상대경로
	int fd; // 반환된 디스크럽터를 담을 변수

	if((fd = creat(fname, CREAT_MODE)) < 0) { // ssu_hole.txt라는 이름으로 파일을 생성하며 권한은 0644로 지정
		// 반환된 디스크럽터 번호가 0보다 작을 시 에러 처리
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1); // 문제가 있을 시 1, 없으면 0, stdlib.h에 정의
	}

	if(write(fd, buf1, 12) != 12) { // 해당 디스크럽터 번호로 buf1에 담긴 내용을 버퍼 크기(12)만큼 쓴다.
		// 정상적으로 리턴되면 버퍼크기를 리턴
		fprintf(stderr, "buf1 write error\n");
		exit(1); // 비정상 종료 처리
	}

	if(lseek(fd, 15000, SEEK_SET) < 0) { // fd에 저장된 디스크럽터 번호로 offset위치를 파일의 처음에서 15000만큼 이동한다.
		// 정상적으로 리턴되면 새로운 파일의 오프셋을 리턴
		fprintf(stderr, "lseek error\n");
		exit(1); // 비정상 종료 처리
	}

	if(write(fd, buf2, 12) != 12) { // 해당 디스크럽터 번호로 buf2에 담긴 내용을 버퍼 크기(12)만큼 쓴다.
		// 정상적으로 리턴되면 버퍼 크기를 리턴
		fprintf(stderr, "buf2 write error\n");
		exit(1); // 비정상 종료 처리
	}

	exit(0); // 정상 종료
}
