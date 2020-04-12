#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;

	if(argc < 2) { // 인자의 개수가 부족할 경우
		fprintf(stderr, "usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
		exit(1);
	}

	for(i = 1; i < argc; i++) { // 인자의 개수만큼 반복
		if(access(argv[i], F_OK) < 0) { // 인자로 주어진 파일들에 접근 여부 확인
			fprintf(stderr, "%s doesn't exist.\n", argv[i]);
			exit(1);
		}

		if(access(argv[i], R_OK) == 0)  // 읽기 권한 확인
			printf("User can read %s\n", argv[i]);
		
		if(access(argv[i], W_OK) == 0) // 쓰기 권한 확인
			printf("User can write %s\n", argv[i]);

		if(access(argv[i], X_OK) == 0) // 실행 권한 확인
			printf("User can execute %s\n", argv[i]);
	}
	exit(0); // 정상 종료
}
