#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SEC_TO_MICRO 100000

int main(int argc, char *argv[])
{
	struct timeval begin_t, end_t;
	gettimeofday(&begin_t, NULL);
	if (argc < 2) {
		fprintf(stderr, "usage: %s <src> <dst>\n", argv[0]);
		exit(1);
	}

	if (link(argv[1], argv[2]) < 0) {
		fprintf(stderr, "link error\n");
		exit(1);
	}

	if (unlink(argv[1]) < 0) {
		fprintf(stderr, "unlink error\n");
		exit(1);
	}
	gettimeofday(&end_t, NULL);

	end_t.tv_sec -=begin_t.tv_sec;
	if(end_t.tv_usec < begin_t.tv_usec) {
		end_t.tv_sec--;
		end_t.tv_usec += SEC_TO_MICRO;
	}
	end_t.tv_usec -= begin_t.tv_usec;
	printf("Runtime : %ld:%ld(sec:microsec)\n", end_t.tv_sec, end_t.tv_usec);
	

	exit(0);
}
