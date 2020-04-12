#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

int main(int argc, char *argv[])
{
	struct ssu_employee record; // ssu_read.c 에서 사용하던 구조체 
	int fd;

	if(argc < 2) { // 인자의 개수가 부족할 경우
		fprintf(stderr, "usage : %s file\n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0640)) < 0) { // 인자로 받은 파일명을 쓰기전용, 없으면 생성 권한은 0640, 있으면 오류로 열기
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	while(1) { // 파일에 입력한 데이터 저장
		printf("Enter employee name <SPACE> salary: "); 
		scanf("%s", record.name);

		if(record.name[0] == '.')
			break;

		scanf("%d", &record.salary);
		record.pid = getpid(); // pid를 넘겨받아 저장 
		write(fd, (char *)&record, sizeof(record)); // 파일에 구조체 데이터를 쓴다
	}

	close(fd); // 디스크럽터 연결 끊기
	exit(0);
}
