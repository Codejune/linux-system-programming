#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	int fd;
	int length;
	char *fname = "ssu_test.txt";
	char buf[BUFFER_SIZE];

	if((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	if(lseek(fd, (off_t)0, SEEK_SET) < 0) {
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

	if(dup2(1, 4) != 4) {
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}

	while(1) {
		length = read(fd, buf, 1024);
		if(length <= 0) break;

		write(4, buf, length);
	}
	exit(0);
}
