#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define WORD_MAX 100

int main(void)
{
	int fd;
	int length = 0, offset = 0, count = 0;
	char *fname = "ssu_test.txt";
	char buf[WORD_MAX][BUFFER_SIZE];
	int i;

	if ((fd = open(fname, O_RDONLY)) < 0) {
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	while ((length = read(fd, buf[count], BUFFER_SIZE)) > 0) {
		for (i = 0; i < length; i++) {
			if (buf[count][i] == '\n') {
				offset = offset + i + 1;
				printf("%s\n", buf[count++]);
				break;
			}
		}
		lseek(fd, offset, SEEK_SET);
	}

	printf("line number : %d\n", count);	
}
