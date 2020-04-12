#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	char buf[BUFFER_SIZE];
	int fd1, fd2;
	int length;

	if(argc != 3) { // 인자의 개수 부족할 경우
		fprintf(stderr, "Usage : %s filein fileout\n", argv[0]);
		exit(1);
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0) { // 첫번째 파일을 읽기전용으로 열기 
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_MODE)) < 0) { // 인자이름으로 파일을 열되, 쓰기전용, 파일이 없을경우 생성하며 이미 존재하면 파일 내용을 지우고 다시 씀
		// 권한은 0644
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	while((length = read(fd1, buf, BUFFER_SIZE)) > 0) // 첫번째 파일에서 버퍼 크기만큼 읽어들임
		write(fd2, buf, length); // 두번째 파일에 버퍼에 담긴 내용을 쓴다

	exit(0);
}
