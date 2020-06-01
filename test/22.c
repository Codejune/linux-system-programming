#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;
	ino_t inode;

	if (argc != 3) {
		fprintf(stderr, "argc !- 3\n");
		exit(1);
	}

	lstat(argv[1], &statbuf);
	inode = statbuf.st_ino;
	printf("%s's inode = %d -> ", argv[1], (int)inode);
	if (link(argv[1], argv[2]) < 0) {
		fprintf(stderr, "link error for %s to %s\n", argv[1], argv[2]);
		exit(1);
	}

	if (unlink(argv[1]) < 0) {
		fprintf(stderr, "unlink error for %s\n", argv[1]);
		exit(1);
	}

	lstat(argv[2], &statbuf);
	inode = statbuf.st_ino;
	printf("%s's inode = %d\n", argv[1], (int)inode);


	exit(0);
}
