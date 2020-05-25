/**
 * @file ssu_crontab.h
 * @brief ssu_crontab.c에서 사용되는 라이브러리, 매크로, 프로토타입 선언
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef SSU_CRONTAB_H // Define Guard
#define SSU_CRONTAB_H

#include <string.h>
#include <ctype.h>
#include <time.h>
#include "common.h"
#include "cron_support.h"

void prompt(void); // 프롬프트 메인
int get_command_type(char *command); // 명령 타입 확인 및 번호 변환
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void to_lower_case(char *str); // 문자열 소문자 변환
void print_reservation_list(int count); // 예약 명령 목록 출력
bool is_period(char *period, int period_type); // 주기 인자 검사
bool is_period_character(char c); // 주기 문자 검사
void write_reservation_file(int count); // 예약 명령 목록 파일 기록
void print_usage(void); // 사용법 출력 

#endif // SSU_CRONTAB_H
