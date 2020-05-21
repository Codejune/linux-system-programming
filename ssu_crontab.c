/*
 * @file ssu_crontab.c
 * @brief ssu_crontab 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_crontab.h"

#define DEBUG

/**
 * @brief ssu_crontab 메인 함수
 */
int main(void)
{
	FILE *fp;
	if (access(CRONTAB_FILE, F_OK) < 0) {
		if ((fp = fopen(CRONTAB_FILE, "w+")) < 0)
			fprintf(stderr, "main: fopen error for %s\n", CRONTAB_FILE);
		else 
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
	char command_buffer[MAX_BUFFER_SIZE]; // 명령행 버퍼
	CommandToken command; // 명령행 토큰 구조체

	while (true) {
		fputs("20162448>", stdout); // 프롬프트 출력 
		fgets(command_buffer, MAX_BUFFER_SIZE, stdin); // 명령행 입력 
		strcpy(command_buffer, ltrim(rtrim(command_buffer))); // 명령행 좌우 공백 제거
		make_command_token(&command, command_buffer); // 명령행 토큰화
		switch (get_command_type(command.argv[0])) {
			case ADD: 
				break;

			case REMOVE: 
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
 * @param command_buffer 명령행 문자열
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
	printf(" ADD    <PERIOD> <EXECUTE COMMAND>    add commands to be excuted periodically in the reservation list\n");
	printf(" REMOVE <COMMAND_NUMBER>              remove the execution reservation of COMMAND_NUMBER from the reservation list\n");
}
