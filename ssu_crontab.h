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

/**
 * @brief 명령어 타입 번호
 */
#define ADD     1
#define REMOVE  2
#define RUN     3
#define EXIT    4
#define UNKNOWN 5

/**
 * @brief 파일 이름
 */
#define CRONTAB_FILE "ssu_crontab_file"
#define CRONTAB_LOG "ssu_crontab_log"

typedef struct CommandTokenStruct // 프롬프트 명령행 토큰 구조체
{
	int argc; // 인자 개수
	char **argv; // 인자 토큰
} CommandToken;

void prompt(void); // 프롬프트 메인
void make_command_token(CommandToken *command, char *command_buffer); // 입력한 명령행을 토큰 구조체로 변환
int get_command_type(char *command); // 명령 타입 확인 및 번호 변환
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void to_lower_case(char *str); // 문자열 소문자 변환
void get_reservation_command(void); // 예약 명령 목록 가져오기
void print_reservation_list(void); // 예약 명령 목록 출력
void write_reservation_file(void); // 예약 명령 목록 파일 기록
void write_log(int command_type, char *command); // 로그 파일에 이력 기록
void free_command_token(CommandToken *command); // 토큰 구조체 메모리 해제
void print_usage(void); // 사용법 출력 

#endif // SSU_CRONTAB_H
