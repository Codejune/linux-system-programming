/**
 * @file ssu_rsync.c
 * @brief ssu_rsync 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_rsync.h"

int main(void)
{
	struct timeval begin_t, end_t;

	gettimeofday(&begin_t, NULL);
	printf("Rsync!\n");
	gettimeofday(&end_t, NULL);
	ssu_runtime(&begin_t, &end_t);
	return 0;
}
