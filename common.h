/**
 * @file common.h
 * @brief 프로젝트에서 공통적으로 사용되는 라이브러리, 매크로 정의
 * @author 김병준 (kbj9704@gmail.com)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#ifndef COMMON_H
#define COMMON_H

/**
 * @brief 버퍼 크기
 */
#define BUFFER_SIZE     256
#define MAX_BUFFER_SIZE 1024

/**
 * @brief 파일 이름
 */
#define CRONTAB_FILE "ssu_crontab_file"
#define CRONTAB_LOG "ssu_crontab_log"

#endif // COMMON_H
