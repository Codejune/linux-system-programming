/**
 * @file 9.c
 * @brief ssu_nonblock1.c
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <fcntl.h>

void set_flags(int fd, int flags);
void clr_flags(int fd, int flags);

char buf[500000];

int main(void) 
{
	int ntowrite, nwrite; 
	char *ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "reading %d bytes\n", ntowrite);
	set_flags(STDOUT_FILENO, O_NONBLOCK);

	ptr = buf; 
	while (ntowrite > 0) {
		errno = 0;

		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

		if (nwrite > 0) {

			ptr += nwrite;
			ntowrite -= nwrite;

		} 
	}

	clr_flags(STDOUT_FILENO, O_NONBLOCK); /* non-blocking 해제 */

	exit(0); 
}

void set_flags(int fd, int flags) 
{
		int val;
		
		if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
			fprintf(stderr, "fcntl F_GETFL failed\n");
			exit(1);;
		}

		val |= flags; /* 플래그 추가 */

		if (fcntl(fd, F_SETFL, val) < 0) {
			fprintf(stderr, "fcntl F_SETFL failed\n");
			exit(1);
		}
}

void clr_flags(int fd, int flags) 
{
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		fprintf(stderr, "fcntl F_GETFL failed\n");
		exit(1);
	}

	val &= ~flags; /* 플래그 제거 */

	if (fcntl(fd, F_SETFL, val) < 0) {
		fprintf(stderr, "fcntl F_SETFL failed\n");
		exit(1);
	}
}
