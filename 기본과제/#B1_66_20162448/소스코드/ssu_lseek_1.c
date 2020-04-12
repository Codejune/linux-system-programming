#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // 표준 심볼 상수 및 자료형, 여기에선 lseek()을 위해 사용
#include <fcntl.h> // open(), fcntl() 등을 위해 사용
#include <sys/types.h> // 자료형 타입 헤더, off_t

int main(void) 
{
	char *fname = "ssu_test.txt"; // open할 파일 이름, 실행 위치 기준 상대경로
	off_t fsize; // long타입의 정수형으로 파일의 크기를 담는 변수
	int fd; // 반환된 디스크럽터를 담을 변수

	if((fd = open(fname, O_RDONLY)) < 0) { // ssu_test.txt파일을 읽기 전용 모드로 open후 반환된 디스크럽터를 fd에 담음 
		// 반환된 디스크럽터 번호가 0보다 작을 시 에러처리
		fprintf(stderr, "open error for %s\n", fname); 
		exit(1); // 문제가 있을 시 1, 없으면 0, stdlib.h에 정의
	}

	if((fsize = lseek(fd, 0, SEEK_END)) < 0) { // fd에 저장된 디스크럽터 번호로 offset 위치를 파일의 끝에서 0만큼 이동한다.
		// 만약 성공적으로 반환되면 해당 파일의 offset을 리턴한다.
		fprintf(stderr, "lseek error\n");
		exit(1); // 문제가 있을 시 1, 없으면 0
	}

	printf("The size of <%s> is %ld bytes.\n", fname, fsize);

	exit(0); // 정상 종료
}
