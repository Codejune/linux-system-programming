#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	char *new_fname = "ssu_test_new.txt";
	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}

	fread(buf, 25, 1, fp);
	buf[25] = 0;
	printf("first printf : %s\n", buf);

	fseek(fp, 1, SEEK_CUR);

	fread(buf + 25 + 1, 24, 1, fp);
	buf[25 + 1 + 24] = 0;
	printf("second printf : %s\n", buf + 25 + 1);

	fclose(fp);

	if ((fp = fopen(new_fname, "w+")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	fwrite(buf, 25, 1, fp);
	fwrite(buf + 25, 24, 1, fp);
	fclose(fp);
	exit(0);
}

