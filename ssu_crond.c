/*
 * @file ssu_crond.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crond.h"

char pwd[BUFFER_SIZE];
char reservation_command[BUFFER_SIZE][MAX_BUFFER_SIZE];

/**
 * @brief ssu_crond 메인 함수
 * @param argc 최초 실행시에 제공되는 인자들의 개수
 * @param argv 최초 실행시에 제공되는 파일들의 경로
 */
int main(int argc, char *argv[])
{
	pthread_t tid[BUFFER_SIZE]; // 스레드 ID 배열
	int reservation_count; // 예약 명령 개수
	int fd; // 예약 명령 파일 디스크럽터
	struct stat statbuf;
	time_t now;

	if (access(CRONTAB_LOG, F_OK) < 0)
		open(CRONTAB_LOG, O_CREAT);

	// 마지막 수정 시간 획득
	fd = open(CRONTAB_FILE, O_RDONLY);
	fstat(fd, &statbuf);
	now = statbuf.st_mtime;

	getcwd(pwd, BUFFER_SIZE);
#ifdef DEBUG
	printf("ssu_crond(): pwd = %s\n", pwd);
#endif

	while (true) {

		reservation_count = get_reservation_command(); // 예약 명령 획득

		for (int i = 0; i < reservation_count; i++) // 획득한 명령 개수만큼의 스레드 시작
#ifdef DEBUG
		{
			printf("ssu_crond(): reservation_command[%d] = %s\n", i, reservation_command[i]);
#endif
			pthread_create(&tid[i], NULL, reservation_execute, (void*)reservation_command[i]); // 스레드 생성, 인덱스 전달
#ifdef DEBUG
			printf("ssu_crond(): tid[%d] = %ld\n", i, tid[i]);
		}
#endif

		while (true) { // 예약 명령 파일 수정 확인
			fstat(fd, &statbuf); // 파일 상태정보 획득 
			if (now != statbuf.st_mtime) { // 파일의 수정시간이 변경되었을 경우
#ifdef DEBUG
				printf("ssu_crond(): %s is modified!\n", CRONTAB_FILE);
#endif
				now = statbuf.st_mtime; // 최종 수정시간 갱신
				for(int i = 0; i < reservation_count; i++)
					pthread_cancel(tid[i]); // 기존 스레드 모두 정지
				break;
			}
		}
	}
}

/**
 * @brief 예약 명령 실행 스레드 
 * @param arg 예약 명령 문자열
 */
void *reservation_execute(void *arg) // 예약 명령 실행 스레드
{
	CommandToken token;
	char copy_reservation_command[MAX_BUFFER_SIZE];
	char command[MAX_BUFFER_SIZE] = { 0 };
	time_t now_t;
	time_t execute_t;
	struct tm now_tm;
	bool *reservation_time_table[5]; // 예약 시간 테이블

	strcpy(copy_reservation_command, (char*)arg);
#ifdef DEBUG
	printf("reservation_excute(): copy_reservation_command = %s\n", copy_reservation_command);
#endif

	// 예약 시간 테이블 생성 및 초기화
	reservation_time_table[0] = calloc(60, sizeof(bool)); // MINUTE
	reservation_time_table[1] = calloc(24, sizeof(bool)); // HOUR
	reservation_time_table[2] = calloc(32, sizeof(bool)); // DAY
	reservation_time_table[3] = calloc(13, sizeof(bool)); // MONTH
	reservation_time_table[4] = calloc(8, sizeof(bool)); // DAY_OF_WEEK
	make_command_token(&token, copy_reservation_command); // 예약 명령 토큰화
	for (int i = 0; i < token.argc; i++) {
		if (i < 5) // 예약 시간 테이블 설정
			set_reservation_time_table(token.argv[i], i + 1, reservation_time_table[i]); 
		else { // 실행 명령어 생성
			strcat(command, token.argv[i]);
			strcat(command, " ");
		}
	}
	//free_command_token(&token);
#ifdef DEBUG
	printf("reservation_execute(): command = %s\n", command);
#endif

	// 현재 시간 추출
	now_t = time(NULL);
	now_tm = *localtime(&now_t);
	now_tm.tm_sec = 0;

	while (true) {

		now_tm.tm_min++;
		execute_t = mktime(&now_tm);
		now_tm = *localtime(&execute_t);
		now_tm.tm_sec = 0;

		if (reservation_time_table[0][now_tm.tm_min] &&
				reservation_time_table[1][now_tm.tm_hour] &&
				reservation_time_table[2][now_tm.tm_mday] &&
				reservation_time_table[3][now_tm.tm_mon + 1] &&
				reservation_time_table[4][now_tm.tm_wday]) { // 실행 시간이 되었을 경우

			now_t = time(NULL);
#ifdef DEBUG
			printf("reservation_execute(): %s, execute waitting for %ld second\n", (char*)arg, execute_t - now_t);
#endif
			sleep(execute_t - now_t); // 명령어 실행 대기
#ifdef DEBUG
			printf("reservation_execute(): %s, running at %s\n", (char*)arg, asctime(&now_tm));
#endif
			system(command); // 명령어 실행
			write_log(RUN, (char*)arg);
		}
	}
}

/**
 * @brief 예약 시간 테이블 설정
 * @param period 주기 문자열
 * @param period_type 주기 타입 번호
 * @param reservation_table 예약 시간 테이블
 */
void set_reservation_time_table(char *period, int period_type, bool *reservation_table) // 예약 시간 테이블 설정
{
	char period_token[BUFFER_SIZE][BUFFER_SIZE];
	char target[BUFFER_SIZE];
	char unit[BUFFER_SIZE];
	char operator;
	char *tmp;
	char *last;
	int period_token_count = 0;
	int begin; // 예약 시간 테이블 시작
	int end; // 예약 시간 테이블 끝

#ifdef DEBUG
	printf("set_reservation_time_table: period = %s, period_type = %d\n", period, period_type);
#endif
	switch (period_type) {
		case MINUTE:
			begin = 0;
			end = 59;
			break;
		case HOUR:
			begin = 0;
			end = 23;
			break;
		case DAY:
			begin = 1;
			end = 31;
			break;
		case MONTH:
			begin = 1;
			end = 12;
			break;
		case DAY_OF_WEEK:
			begin = 0;
			end = 6;
			break;
	}

	// 1. 쉼표(,) 기준 분리
	tmp = strtok_r(period, ",", &last);
	strcpy(period_token[period_token_count++], tmp);
	while ((tmp = strtok_r(NULL, ",", &last)) != NULL)
		strcpy(period_token[period_token_count++], tmp);

	// 2. 토큰에 해당하는 시간을 설정
	for (int i = 0; i < period_token_count; i++) {

		// 임시 변수 초기화
		operator = 0;
		memset(target, 0, BUFFER_SIZE);
		memset(unit, 0, BUFFER_SIZE);

		sscanf(period_token[i], "%[^/(-)]%c%s", target, &operator, unit); // 슬래시(/) 우선 토큰 분리
#ifdef DEBUG
		printf("set_reservation_time_table: target = %s, operator = %c, unit = %s\n", target, operator, unit);
#endif
		// 3. 슬래시(/)가 존재하지 않을 경우: 범위
		if (operator == '-') {

			for (int j = atoi(target); j <= atoi(unit); j++)
				reservation_table[j] = true;

		} else if (operator == '/') { // 4. 슬래시(/)가 존재할 경우

			int count = atoi(unit);

			// 4-1. 앞 토큰이 범위일 경우 
			if (strchr(target, '-') != NULL) {
				char front_target[BUFFER_SIZE] = { 0 };
				char front_unit[BUFFER_SIZE] = { 0 };
				char front_operator = 0;
				sscanf(target, "%[^-]%c%s", front_target, &front_operator, front_unit);
#ifdef DEBUG
				printf("is_period(): front_target = %s, front_operator = %c, front_unit = %s\n", front_target, front_operator, front_unit);
#endif
				for (int j = atoi(front_target); j <= atoi(front_unit); j++) 
					if(count++ % atoi(unit) == 0)
						reservation_table[j] = true;

			} else { // 4-2. 앞 토큰이 범위가 아닐 경우

				for (int j = begin; j <= end; j++)
					if(count++ % atoi(unit) == 0)
						reservation_table[j] = true;
			}
		} else { // 5. 숫자 혹은 전체(*)일 경우

			if (strchr(target, '*') == NULL) // 숫자일 경우
				reservation_table[atoi(target)] = true;
			else // 전체(*)일 경우
				for (int j = begin; j <= end; j++)
					reservation_table[j] = true;
		}
	}

}
