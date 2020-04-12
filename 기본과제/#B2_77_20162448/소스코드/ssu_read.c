#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h" // 사용자 정의 헤더

int main(int argc, char *argv[])
{
	struct ssu_employee record; // ssu_employee.h 선언
	int fd; // 파일 디스크럽터
	int record_num; 

	if (argc < 2) {	// 명령어 인자 개수 부족
		fprintf(stderr, "Usage : %s file\n", argv[0]);
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) { // ssu_employeefile 읽기전용 열기
		fprintf(stderr, "open error for %s\n", argv[1]); // 열기 오류
		exit(1);
	}

	while(1) {
		printf("Enter rercord number : ");
		scanf("%d", &record_num);

		if(record_num < 0) // 음수시 프로그램 종료
			break;
		
		// 데이터 탐색, 파일의 처음에서 입력한 수 만큼 offset 이동
		if(lseek(fd, (long)record_num * sizeof(record), 0) < 0) {
			fprintf(stderr, "lseek error\n"); // 탐색 오류
			exit(1);
		}

		// 데이터 읽기, offset위치에서 ssu_imployee구조체의 시작인 char포인터로 구조체 크기만큼 읽음
		if(read(fd, (char *)&record, sizeof(record)) > 0)
			printf("Employee : %s Salary : %d\n", record.name, record.salary);
		else // 데이터 없음 
			printf("Record %d not found\n", record_num);
	}

	close(fd); // 디스크럽터 연결 끊기
	exit(0); // 정상 종료
}


























		
