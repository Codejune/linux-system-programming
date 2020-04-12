#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	int fd1, fd2;
	char *buf;
	off_t end;

	if(argc != 3) {
		fprintf(stderr, "Usage : %s, filein fileout\n", argv[0]);
		exit(1);
	}

	if((fd1 = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	if((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0666)) < 0) {
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	if((end = lseek(fd1, (off_t)0, SEEK_END)) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

	lseek(fd1, (off_t)0, SEEK_SET);
	buf = (char *)calloc((int)end, sizeof(char));
	if(read(fd1, buf, end) < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}

	if(write(fd2, buf, (int)end) < 0) {
		fprintf(stderr, "write error\n");
		exit(1);
	}
	exit(0);
}
