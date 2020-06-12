#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char *argv[])
{
	int fdin, fdout;
	void *src, *dst;
	struct stat statbuf;

	if (argc != 3) {
		printf("usage : %s <fromfile> <tofile>\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) { /* fromfile 열기 */
		printf("can't open %s for reading\n", argv[1]);
		exit(1);
	}

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) { /* tofile 열기 */
		printf("cannot creat %s for writing\n", argv[2]);
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0) { /* 입력 파일의 크기 획득 */
		printf("fstat() error\n");
		exit(1);
	}

	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) { /* 출력 파일의 크기 조정 */
		printf("lseek() error\n");
		exit(1);
	}

	if (write(fdout, "", 1) != 1) { /* 쓰기 가능 여부 검사 */
		printf("write() error");
		exit(1);
	}

	/* mmap: 메모리 맵 입출력, 특정 크기만큼의 메모리를 지정, 시작주소와 길이를 인자로 주며, 영역보호방식(PROT), 특성(FLAG)를 통해 메모리를 맵핑한다 */
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) { /* ADDR, LENGTH, PROT, FLAG, FD, OFFSET */
		printf("mmap() error for input\n");
		exit(1);
	}

	if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED) {
		printf("mmap() error for output\n");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); /* 파일 복사를 수행 */
	exit(0);
}


