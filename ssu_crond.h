/*
 * @file ssu_crond.h
 * @brief ssu_crond.c에서 사용되는 라이브러리, 매크로, 프로토타입 선언
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef SSU_CROND_H
#define SSU_CROND_H

#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "cron_support.h"

#define SSU_CROND_SERVICE ssu_crond.service

void *reservation_execute(void *arg); // 예약 명령 실행 스레드
void set_reservation_time_table(char *period, int period_type, bool *reservation_table); // 예약 시간 테이블 설정
#endif // SSU_CROND_H
