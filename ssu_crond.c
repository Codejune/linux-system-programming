/*
 * @file ssu_crond.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crond.h"

#define DEBUG

char pwd[BUFFER_SIZE];
char reservation_command[BUFFER_SIZE][MAX_BUFFER_SIZE];
int reservation_count = 0;

/**
 * @brief ssu_crond 메인 함수
 */
int main(void)
{
	getcwd(pwd, BUFFER_SIZE);
#ifdef DEBUG
	printf("main(): pwd = %s\n", pwd);
#endif
	exit(0);
}

