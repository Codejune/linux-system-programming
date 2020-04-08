#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	char *new_fname = "ssu_test_new.txt";
	int fd;
	FILE *fp;

	fd = open(fname, O_RDONLY);
	fp = fopen(new_fname, "w");
	if(fd < 0 || fp == NULL){
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	read(fd, buf, 25);
	buf[25] = 0;
	printf("first printf : %s\n", buf);
	lseek(fd, 1, SEEK_CUR);
	read(fd, buf+25+1, 24);
	buf[25+1+24] = 0;
	printf("second printf : %s\n", buf+25+1);
	close(fd);
	fwrite(buf, 25, 1, fp);
	fwrite(buf+25, 24, 1, fp);
	fclose(fp);
	exit(0);

	//위 코드를 아래 코드로 변경시 정답
	fp = fopen(fname, "r");
	fd = open(new_fname, O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd < 0 || fp == NULL){
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	fread(buf, 25, 1, fp);
	buf[25] = 0;
	printf("first printf : %s\n", buf);
	fseek(fp, 1, SEEK_CUR);
	fread(buf+25+1, 24, 1, fp);
	buf[25+1+24] = 0;
	printf("second printf : %s\n", buf+25+1);
	fclose(fp);
	write(fd, buf, 25);
	write(fd, buf+25, 24);
	close(fd);
	exit(0);
}