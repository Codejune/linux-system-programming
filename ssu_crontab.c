/*
 * @file ssu_crontab.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crontab.h"

#define DEBUG

char reservation_command[BUFFER_SIZE][MAX_BUFFER_SIZE];
int reservation_count = 0;

/**
 * @brief ssu_crontab 메인 함수
 */
int main(void)
{
	FILE *fp;

	if (access(CRONTAB_FILE, F_OK) < 0) { // 예약 명령 목록 파일 확인
		if ((fp = fopen(CRONTAB_FILE, "w+")) < 0) // 존재하지 않을 경우 생성
			fprintf(stderr, "main: fopen error for %s\n", CRONTAB_FILE);
		fclose(fp);
	}

	if (access(CRONTAB_LOG, F_OK) < 0) { // 로그 파일 확인
		if ((fp = fopen(CRONTAB_LOG, "w+")) < 0) // 존재하지 않을 경우 생성
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
	bool is_invalid;

	// REMOVE
	int command_number;
	char tmp[MAX_BUFFER_SIZE];

	while (true) {
		get_reservation_command();
		print_reservation_list();
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
					if (i < 6) // PERIOD 오류 탐색
						if (command.argv[i][0] < '0' || command.argv[i][0] > '9')
							if(command.argv[i][0] != '*') {
								is_invalid = true;
								break;
							}

					if (i == 1) {
						sprintf(command_buffer, "%s", command.argv[i]);
						continue;
					}
					sprintf(command_buffer, "%s %s", command_buffer, command.argv[i]);
				}

				if (is_invalid) {
					fprintf(stderr, "prompt.ADD: invalid PERIOD\n");
					break;
				}
#ifdef DEBUG
				printf("prompt().ADD: command_buffer = %s\n", command_buffer);
#endif
				strcpy(reservation_command[reservation_count], command_buffer); // 예약 목록 배열에 추가
				write_reservation_file(); // 예약 목록 파일 작성
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
				write_reservation_file();
				write_log(REMOVE, command_buffer);
				break;

			case EXIT: 
				exit(0);
				break;

			case UNKNOWN:
				print_usage();
				break;
		}
		free_command_token(&command);
	}
}

/**
 * @brief 입력한 명령행을 토큰 구조체로 변환
 * @param command 명령행 토큰 구조체
 * @param command_buffer 명령 문자열
 */
void make_command_token(CommandToken *command, char *command_buffer) // 입력한 명령행을 토큰 구조체로 변환
{
	char *tmp;

	command->argv = (char**)calloc(BUFFER_SIZE, sizeof(char*));
	command->argc = 0;

	if ((tmp = strtok(command_buffer, " ")) == NULL)
		return;
#ifdef DEBUG
	printf("make_command_token(): command->argv[%d] = %s\n", command->argc, tmp);
#endif

	command->argv[command->argc] = (char *)calloc(BUFFER_SIZE, sizeof(char));
	strcpy(command->argv[command->argc++], tmp);

	while ((tmp = strtok(NULL, " ")) != NULL) {
#ifdef DEBUG
		printf("make_command_token(): command->argv[%d] = %s\n", command->argc, tmp);
#endif
		command->argv[command->argc] = (char *)calloc(BUFFER_SIZE, sizeof(char)); // 명령행 인자 메모리 공간 할당
		strcpy(command->argv[command->argc++], tmp);
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
 * @brief 예약 명령 목록 가져오기
 */
void get_reservation_command(void) // 예약 명령 목록 가져오기
{
	FILE *fp;

	reservation_count = 0;

	if ((fp = fopen(CRONTAB_FILE, "r+")) < 0) {
		fprintf(stderr, "print_crontab_file: fopen error for %s\n", CRONTAB_FILE);
		return;
	}

	while(fscanf(fp, "%[^\n]\n", reservation_command[reservation_count]) > 0)
		reservation_count++;
#ifdef DEBUG
	printf("get_reservation_command: reservation_count = %d\n", reservation_count);
#endif

	fclose(fp);

}

/**
 * @brief 예약 명령 목록 출력
 */
void print_reservation_list(void) // 예약 명령 목록 출력
{
	for(int i = 0; i < reservation_count; i++)
		printf("%d. %s\n", i, reservation_command[i]);
	printf("\n");
}

/**
 * @brief 예약 명령 목록 파일 기록
 */
void write_reservation_file(void) // 예약 명령 목록 파일 기록
{
	FILE *fp;

	if ((fp = fopen(CRONTAB_FILE, "w+")) < 0) { // 예약 명령 목록 파일 열기
		fprintf(stderr, "prompt: fopen error for %s\n", CRONTAB_FILE);
		return;
	}
	fseek(fp, 0, SEEK_SET);

	for(int i = 0; i <= reservation_count; i++)
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
 * @brief 로그 파일에 이력 기록
 * @param command_type 명령 타입 번호
 * @param command 명령 문자열
 */
void write_log(int command_type, char *command) // 로그 파일에 이력 기록
{
	FILE *fp;
	time_t now_t;
	struct tm *now_tm;

	if ((fp = fopen(CRONTAB_LOG, "r+")) < 0) {
		fprintf(stderr, "write_log: fopen error for %s\n", CRONTAB_LOG);
		return;
	}
	fseek(fp, 0, SEEK_END);

	time(&now_t);
	now_tm = localtime(&now_t);

	switch (command_type) {
		case ADD:
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "add", command);
			break;
		case REMOVE:
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "remove", command);
			break;
		case RUN:
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "run", command);
			break;
	}
	fclose(fp);
}

/**
 * @brief 명령행 구조체 초기화
 * @param command 명령행 구조체
 */
void free_command_token(CommandToken *command) // 명령행 구조체 초기화
{
	int i;

	for (i = 0; i < command->argc; i++)
		free(command->argv[i]);
	free(command->argv);
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
