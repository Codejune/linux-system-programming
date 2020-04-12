#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat file_info;
	char *str;
	int i;

	for(i = 1; i < argc; i++) { // 인자의 개수
		printf("name = %s, type = ", argv[i]);

		if(lstat(argv[i], &file_info) < 0) { // 인자로 넘겨받은 파일의 정보를 file_info 구조체에 저장
			fprintf(stderr, "lstat error\n");
			continue;
		}

		// 표준 테스트 매크로 확인
		if(S_ISREG(file_info.st_mode)) // 일반 파일일 경우
			str = "regular";
		else if(S_ISDIR(file_info.st_mode)) // 디렉토리일 경우
			str = "directory";
		else if(S_ISCHR(file_info.st_mode)) // 문자열일 경우
			str = "character special";
		else if(S_ISBLK(file_info.st_mode)) // 블록일 경우
		       	str = "block special";
		else if(S_ISFIFO(file_info.st_mode)) // 파이프일 경우
			str = "FIFO";
		else if(S_ISLNK(file_info.st_mode)) // 심링크 일 경우
			str = "symbolic link";
		else if(S_ISSOCK(file_info.st_mode)) // 소켓일 경우
			str = "socket";
		else
			str = "unknown mode";

		printf("%s\n", str);
	}
	exit(0);
}
