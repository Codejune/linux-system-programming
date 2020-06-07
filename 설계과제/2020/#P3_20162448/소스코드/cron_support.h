/**
 * @file cron_support.h
 * @brief ssu_crontab과 ssu_crond에서 공통적으로 사용되는 라이브러리, 매크로 정의
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef CRON_SUPPORT_H
#define CRON_SUPPORT_H

#include <pthread.h>
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
 * @brief 주기 타입 번호
 */
#define MINUTE      1
#define HOUR        2
#define DAY         3
#define MONTH       4
#define DAY_OF_WEEK 5

/**
 * @brief 파일 이름
 */
#define CRONTAB_FILE "ssu_crontab_file"
#define CRONTAB_LOG "ssu_crontab_log"

/**
 * @brief 프롬프트 명령행 토큰 구조체
 * @param argc 인자 개수
 * @param argv 인자 토큰
 */
typedef struct CommandTokenStruct
{
	int argc; // 인자 개수
	char **argv; // 인자 토큰
} CommandToken;

void make_command_token(CommandToken *command, char *command_buffer); // 입력한 명령행을 토큰 구조체로 변환
void free_command_token(CommandToken *command); // 토큰 구조체 메모리 해제
int get_reservation_command(void); // 예약 명령 목록 가져오기
void write_log(int command_type, char *command); // 로그 파일에 이력 기록

#endif // CRON_SUPPORT_H
