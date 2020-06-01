#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	struct stat statbuf_1, statbuf_2;

	if (argc != 3) {
		fprintf(stderr, "usage : %s <filename1> <filename2>\n", argv[0]);
		exit(1);
	}
	
	if (lstat(argv[1], &statbuf_1) < 0) {
		fprintf(stderr, "lstat error for %s\n", argv[1]);
		exit(1);
	}

	if (lstat(argv[2], &statbuf_2) < 0) {
		fprintf(stderr, "lstat error for %s\n", argv[2]);
		exit(1);
	}

	if (!S_ISREG(statbuf_1.st_mode)) {
		fprintf(stderr, "%s doesn't regular file\n", argv[1]);
		exit(1);
	}

	if (!S_ISREG(statbuf_2.st_mode)) {
		fprintf(stderr, "%s doesn't regular file\n", argv[2]);
		exit(1);
	}
	strcpy(buf, "diff ");
	strcat(buf, argv[1]);
	strcat(buf, " ");
	strcat(buf, argv[2]);
	system(buf);
	exit(0);
}
