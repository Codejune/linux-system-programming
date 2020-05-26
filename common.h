/**
 * @file common.h
 * @brief 프로젝트에서 공통적으로 사용되는 라이브러리, 매크로 정의
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef COMMON_H
#define COMMON_H

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// 버퍼 크기
#define BUFFER_SIZE     256
#define MAX_BUFFER_SIZE 1024

// 시간 크기
#define SECOND_TO_MICRO 100000

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t); // 실행시간 출력 

#endif // COMMON_H
