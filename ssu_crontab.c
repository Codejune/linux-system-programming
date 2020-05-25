/*
 * @file ssu_crontab.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crontab.h"

#define DEBUG

char reservation_command[BUFFER_SIZE][MAX_BUFFER_SIZE];

/**
 * @brief ssu_crontab 메인 함수
 */
int main(void)
{
	FILE *fp;

	if (access(CRONTAB_FILE, F_OK) < 0) { // 예약 명령 목록 파일 확인
		if ((fp = fopen(CRONTAB_FILE, "w+")) == NULL) // 존재하지 않을 경우 생성
			fprintf(stderr, "main: fopen error for %s\n", CRONTAB_FILE);
		fclose(fp);
	}

	if (access(CRONTAB_LOG, F_OK) < 0) { // 로그 파일 확인
		if ((fp = fopen(CRONTAB_LOG, "w+")) == NULL) // 존재하지 않을 경우 생성
			fprintf(stderr, "main: fopen error for %s\n", CRONTAB_LOG);
		fclose(fp);
	}

	prompt();
	exit(0); // 정상 종료
}

/**
 * @brief 프롬프트 메인
 */
void prompt(void) // 프롬프트 메인
{
	// PROMPT
	char command_buffer[MAX_BUFFER_SIZE]; // 명령행 버퍼
	CommandToken command; // 명령행 토큰 구조체

	// COMMON
	int reservation_count;
	bool is_invalid;

	// REMOVE
	int command_number;
	char tmp[MAX_BUFFER_SIZE];

	while (true) {
		reservation_count = get_reservation_command();
		print_reservation_list(reservation_count);
		fputs("20162448>", stdout); // 프롬프트 출력 
		fgets(command_buffer, MAX_BUFFER_SIZE, stdin); // 명령행 입력
		strcpy(command_buffer, ltrim(rtrim(command_buffer))); // 명령행 좌우 공백 제거
		make_command_token(&command, command_buffer); // 명령행 토큰화
		switch (get_command_type(command.argv[0])) {

			case ADD: 

				is_invalid = false;
				if(command.argc < 7) { // 인자 개수가 부족할 경우
					print_usage();
					break;
				}

				for (int i = 1; i < command.argc; i++) { // 예약 명령 문자열 생성

					// PERIOD 오류 검사
					if (i < 6) 
						if (!is_period(command.argv[i], i)) {
							is_invalid = true;
							break;
						}

					if (i == 1) {
						sprintf(command_buffer, "%s", command.argv[i]);
						continue;
					}
					strcat(command_buffer, " ");
					strcat(command_buffer, command.argv[i]);
				}

				if (is_invalid) {
					fprintf(stderr, "prompt.ADD: invalid PERIOD\n");
					break;
				}
#ifdef DEBUG
				printf("prompt().ADD: command_buffer = %s\n", command_buffer);
#endif
				strcpy(reservation_command[reservation_count], command_buffer); // 예약 목록 배열에 추가
				write_reservation_file(reservation_count); // 예약 목록 파일 작성
				write_log(ADD, command_buffer);
				break;

			case REMOVE: 

				if (command.argc < 2) {
					print_usage();
					break;
				}

				command_number = atoi(command.argv[1]);
				if (command_number < 0 || command_number > reservation_count) {
					fprintf(stderr, "prompt: invalid COMMAND_NUMBER");
					return;
				}

				sprintf(command_buffer, "%s", reservation_command[command_number]);
#ifdef DEBUG
				printf("prompt().REMOVE: command_buffer = %s\n", command_buffer);
#endif

				for (int i = command_number; i < reservation_count; i++) { // 삭제할 예약 명령을 맨 끝으로 이동
					strcpy(tmp, reservation_command[i]);
					strcpy(reservation_command[i], reservation_command[i + 1]);
					strcpy(reservation_command[i + 1], tmp);
				}

				reservation_count--;
				write_reservation_file(reservation_count);
				write_log(REMOVE, command_buffer);
				break;

			case EXIT:
				return;

			case UNKNOWN:
				print_usage();
				break;
		}
		free_command_token(&command);
	}
}

/**
 * @brief 명령 타입 확인 및 번호 변환
 * @param command 명령 문자열
 * @return 명령 타입 번호
 */
int get_command_type(char *command) // 명령 타입 확인 및 번호 변환
{
#ifdef DEBUG
	printf("get_command_type(): command = %s\n", command);
#endif
	if (command == NULL)
		return false;
	else if (!strcmp(command, "add"))
		return ADD;
	else if (!strcmp(command, "remove"))
		return REMOVE;
	else if (!strcmp(command, "exit"))
		return EXIT;
	else
		return UNKNOWN;
}

/**
 * @brief 문자열 오른쪽 공백 제거
 * @param _str 대상 문자열
 * @return 공백이 제거된 문자열
 */
char *rtrim(char *_str) // 문자열 오른쪽 공백 제거
{
	char tmp[MAX_BUFFER_SIZE];
	char *end;

	strcpy(tmp, _str);
	end = tmp + strlen(tmp) - 1;
	while (end != _str && isspace(*end))
		--end;

	*(end + 1) = '\0';
	_str = tmp;
	return _str;
}

/**
 * @brief 문자열 왼쪽 공백 제거
 * @param _str 대상 문자열
 * @return 공백이 제거된 문자열
 */
char *ltrim(char *_str) // 문자열 왼쪽 공백 제거
{
	char *start = _str;

	while (*start != '\0' && isspace(*start))
		++start;
	_str = start;
	return _str;
}

void to_lower_case(char *str) // 문자열 소문자 변환
{
	int i = 0;

	while (str[i]) {
		if (str[i] >= 'A' && str[i] <= 'Z'){
			str[i] = str[i]+32;
		}
		i++;
	}
}

/**
 * @brief 예약 명령 목록 출력
 */
void print_reservation_list(int count) // 예약 명령 목록 출력
{
	for(int i = 0; i < count; i++)
		printf("%d. %s\n", i, reservation_command[i]);
	printf("\n");
}

/**
 * @brief 주기 인자 검사
 * @param period 주기 문자열
 * @param period_type 주기 타입 번호
 */
bool is_period(char *period, int period_type) // 주기 인자 검사
{

	int length = strlen(period);
	char period_buffer[MAX_BUFFER_SIZE];
	char period_token[BUFFER_SIZE][BUFFER_SIZE];
	int period_token_count = 0;
	char *tmp;
	char target[BUFFER_SIZE];
	char unit[BUFFER_SIZE];
	char operator;

	// 복사본 문자열 생성
	strcpy(period_buffer, period);

	// 주기 문자 검사
	for(int i = 0; i < length; i++) {
		if (!is_period_character(period_buffer[i])) 
			return false;
	}

	// 1-1. 쉼표(,) 분리
	if ((tmp = strtok(period_buffer, ",")) == NULL) // 쉼표 앞에 아무것도 존재하지 않을 경우
		return false;

	// 1-2. 맨 앞에 기호만 나왔을 경우
	if (tmp[0] == '-' || tmp[0] == '/')
		return false;
#ifdef DEBUG
	printf("is_period(): type = %d, period_token[%d] = %s\n", period_type, period_token_count, tmp);
#endif
	strcpy(period_token[period_token_count++], tmp);
	while ((tmp = strtok(NULL, ",")) != NULL) 
#ifdef DEBUG
	{
		// 1-2. 맨 앞에 기호만 나왔을 경우
		if (tmp[0] == '-' || tmp[0] == '/')
			return false;
		printf("is_period(): type = %d, period_token[%d] = %s\n", period_type, period_token_count, tmp);
		strcpy(period_token[period_token_count++], tmp);
	}
#else
	strcpy(period_token[period_token_count++], tmp);
#endif

	// 2. 슬래쉬(/), 바(-) 분리
	for(int i = 0; i < period_token_count; i++) {
		operator = 0;
		memset(target, 0, BUFFER_SIZE);
		memset(unit, 0, BUFFER_SIZE);
		sscanf(period_token[i], "%[^/(-)]%c%s", target, &operator, unit); // 슬래시 우선 토큰 분리
#ifdef DEBUG
		printf("is_period(): target = %s, operator = %c, unit = %s\n", target, operator, unit);
#endif
	}

	// 3. 슬래시(/)가 존재하지 않을 경우 범위 확인
	if (operator == '-') {

		// 3-1. 중복된 바(-)가 존재할 경우
		if (strchr(unit, '-') != NULL)
			return false;

		// 3-2. 시작, 끝범위가 전체(*)로 끝날 경우
		if (strchr(target, '*') != NULL || strchr(unit, '*') != NULL) 
			return false;

		// 3-3. 뒤의 수가 앞의 수보다 클 경우
		if (atoi(target) < atoi(unit)) 
			return false;

		// 3-4 허용 범위를 초과했을 경우
		switch (period_type) {
			case MINUTE:

				if (atoi(target) < 0 || atoi(target) > 59) // 앞의 수가 범위를 초과할 경우
					return false;
				else if (atoi(unit) < 0 || atoi(unit) > 59) // 뒤의 수가 범위를 초과할 경우
					return false;
				break;

			case HOUR:

				if (atoi(target) < 0 || atoi(target) > 23) // 앞의 수가 범위를 초과할 경우
					return false;
				else if (atoi(unit) < 0 || atoi(unit) > 23) // 뒤의 수가 범위를 초과할 경우
					return false;
				break;

			case DAY:

				if (atoi(target) < 1 || atoi(unit) > 31) // 앞의 수가 범위를 초과할 경우
					return false;
				else if (atoi(unit) < 1 || atoi(unit) > 31) // 뒤의 수가 범위를 초과할 경우
					return false;
				break;

			case MONTH:

				if (atoi(target) < 1 || atoi(target) > 12) // 앞의 수가 범위를 초과할 경우
					return false;
				else if (atoi(unit) < 1 || atoi(unit) > 12) // 뒤의 수가 범위를 초과할 경우
					return false;
				break;

			case DAY_OF_WEEK:

				if (atoi(target) < 0 || atoi(target) > 6) // 앞의 수가 범위를 초과할 경우
					return false;
				else if (atoi(unit) < 0 || atoi(unit) > 6) // 뒤의 수가 범위를 초과할 경우
					return false;
				break;
		}	

	} else if (operator == '/') { // 4. 슬래시(/)가 존재할 경우 범위 확인

		// 4-1. 중복된 슬래시(/) 또는 바(-)가 존재할 경우
		if (strchr(unit, '/') != NULL || strchr(unit, '-') != NULL) 
			return false;

		// 4-2. 시작, 끝의 범위가 전체(*)로 끝날 경우
		if ((strchr(target, '*') != NULL && strlen(unit) != 1) || strchr(unit, '*') != NULL) 
			return false;

		// 4-3. 앞 토큰이 범위일 경우 
		if (strchr(target, '-') != NULL) {
			char front_target[BUFFER_SIZE] = { 0 };
			char front_unit[BUFFER_SIZE] = { 0 };
			char front_operator = 0;

			sscanf(target, "%[^-]%c%s", front_target, &front_operator, front_unit);
#ifdef DEBUG
			printf("is_period(): front_target = %s, front_operator = %c, front_unit = %s\n", front_target, front_operator, front_unit);
#endif

			// 4-4. 중복된 바(-)가 존재할 경우
			if (strchr(front_unit, '-') != NULL)
				return false;

			// 4-5. 시작, 끝범위가 전체(*)로 끝날 경우
			if (strchr(front_target, '*') != NULL || strchr(front_unit, '*') != NULL) 
				return false;

			// 4-6. 앞의 수가 뒤의 수보다 작을 경우
			if (atoi(front_target) > atoi(front_unit)) 
				return false;

			// 4-7 허용 범위를 초과했을 경우
			switch (period_type) {
				case MINUTE:

					if (atoi(front_target) < 0 || atoi(front_target) > 59) // 앞의 수가 범위를 초과할 경우
						return false;
					else if (atoi(front_unit) < 0 || atoi(front_unit) > 59) // 뒤의 수가 범위를 초과할 경우
						return false;
					break;

				case HOUR:

					if (atoi(front_target) < 0 || atoi(front_target) > 23) // 앞의 수가 범위를 초과할 경우
						return false;
					else if (atoi(front_unit) < 0 || atoi(front_unit) > 23) // 뒤의 수가 범위를 초과할 경우
						return false;
					break;

				case DAY:

					if (atoi(front_target) < 1 || atoi(front_target) > 31) // 앞의 수가 범위를 초과할 경우
						return false;
					else if (atoi(front_unit) < 1 || atoi(front_unit) > 31) // 뒤의 수가 범위를 초과할 경우
						return false;
					break;

				case MONTH:

					if (atoi(front_target) < 1 || atoi(front_target) > 12) // 앞의 수가 범위를 초과할 경우
						return false;
					else if (atoi(front_unit) < 1 || atoi(front_unit) > 12) // 뒤의 수가 범위를 초과할 경우
						return false;
					break;

				case DAY_OF_WEEK:

					if (atoi(front_target) < 0 || atoi(front_target) > 6) // 앞의 수가 범위를 초과할 경우
						return false;
					else if (atoi(front_unit) < 0 || atoi(front_unit) > 6) // 뒤의 수가 범위를 초과할 경우
						return false;
					break;

			} 
		} else { // 4-8. 앞 토큰이 범위가 아닐 경우

			// 4-9. 시작 범위가 숫자일 때 허용 범위 초과했을 경우
			if (strchr(target, '*') == NULL)
				switch (period_type) {
					case MINUTE:

						if (atoi(target) < 0 || atoi(target) > 59) // 앞의 수가 범위를 초과할 경우
							return false;
						break;

					case HOUR:

						if (atoi(target) < 0 || atoi(target) > 23) // 앞의 수가 범위를 초과할 경우
							return false;
						break;

					case DAY:

						if (atoi(target) < 1 || atoi(target) > 31) // 앞의 수가 범위를 초과할 경우
							return false;
						break;

					case MONTH:

						if (atoi(target) < 1 || atoi(target) > 12) // 앞의 수가 범위를 초과할 경우
							return false;
						break;

					case DAY_OF_WEEK:

						if (atoi(target) < 0 || atoi(target) > 6) // 앞의 수가 범위를 초과할 경우
							return false;
						break;
				}

			// 4-10. 끝 범위가 숫자일 때 허용 범위 초과했을 경우
			switch (period_type) {
				case MINUTE:

					if (atoi(unit) < 0 || atoi(unit) > 59) // 앞의 수가 범위를 초과할 경우
						return false;
					break;

				case HOUR:

					if (atoi(unit) < 0 || atoi(unit) > 23) // 앞의 수가 범위를 초과할 경우
						return false;
					break;

				case DAY:

					if (atoi(unit) < 1 || atoi(unit) > 31) // 앞의 수가 범위를 초과할 경우
						return false;
					break;

				case MONTH:

					if (atoi(unit) < 1 || atoi(unit) > 12) // 앞의 수가 범위를 초과할 경우
						return false;
					break;

				case DAY_OF_WEEK:

					if (atoi(unit) < 0 || atoi(unit) > 6) // 앞의 수가 범위를 초과할 경우
						return false;
					break;
			}
		}
	}
	return true;
}

/**
 * @brief 주기 문자 검사
 * @param c 주기 문자
 */
bool is_period_character(char c) // 주기 문자 검사
{
	if (c == '*')
		return true;
	else if (c == '-')
		return true;
	else if (c == '/')
		return true;
	else if (c == ',')
		return true;
	else if (isdigit(c))
		return true;
	else
		return false;
}

/**
 * @brief 예약 명령 목록 파일 기록
 */
void write_reservation_file(int count) // 예약 명령 목록 파일 기록
{
	FILE *fp;

	if ((fp = fopen(CRONTAB_FILE, "w+")) == NULL) { // 예약 명령 목록 파일 열기
		fprintf(stderr, "prompt: fopen error for %s\n", CRONTAB_FILE);
		return;
	}
	fseek(fp, 0, SEEK_SET);

	for(int i = 0; i <= count; i++)
#ifdef DEBUG
	{
		printf("write_reservation_file: reservation_command[%d] = %s\n", i, reservation_command[i]);
		fprintf(fp, "%s\n", reservation_command[i]);
	}
#else
	fprintf(fp, "%s\n", reservation_command[i]);
#endif

	fclose(fp);
}

/**
 * @brief 사용법 출력
 */
void print_usage(void) // 사용법 출력 
{
	printf("Usage: [COMMAND]\n");
	printf("Command : \n");
	printf(" add    <PERIOD> <EXECUTE COMMAND>    add commands to be excuted periodically in the reservation list\n");
	printf(" remove <COMMAND_NUMBER>              remove the execution reservation of COMMAND_NUMBER from the reservation list\n");
}
