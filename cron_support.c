/**
 * @file cron_support.c
 * @brief ssu_crontab, ssu_crond에서 공통적으로 사용되는 함수 
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "cron_support.h"

FILE *fp;
pthread_mutex_t mutex; // 뮤텍스 객체 선언
extern char reservation_command[BUFFER_SIZE][MAX_BUFFER_SIZE]; // 예약 명령 목록

/**
 * @brief 입력한 명령행을 토큰 구조체로 변환
 * @param command 명령행 토큰 구조체
 * @param command_buffer 명령 문자열
 */
void make_command_token(CommandToken *command, char *command_buffer) // 입력한 명령행을 토큰 구조체로 변환
{
	char *tmp;
	char *last;
#ifdef DEBUG
	printf("make_command_token(): command_buffer = %s\n", command_buffer);
#endif
	command->argv = (char**)calloc(BUFFER_SIZE, sizeof(char*));
	command->argc = 0;

	if ((tmp = strtok_r(command_buffer, " ", &last)) == NULL)
		return;
#ifdef DEBUG
	printf("make_command_token(): command->argv[%d] = %s\n", command->argc, tmp);
#endif

	command->argv[command->argc] = (char *)calloc(BUFFER_SIZE, sizeof(char));
	strcpy(command->argv[command->argc++], tmp);

	while ((tmp = strtok_r(NULL, " ", &last)) != NULL) {
#ifdef DEBUG
		printf("make_command_token(): command->argv[%d] = %s\n", command->argc, tmp);
#endif
		command->argv[command->argc] = (char *)calloc(BUFFER_SIZE, sizeof(char)); 
		strcpy(command->argv[command->argc++], tmp);
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
 * @brief 예약 명령 목록 가져오기
 */
int get_reservation_command(void) // 예약 명령 목록 가져오
{
	FILE *fp;

	int count = 0;

	if ((fp = fopen(CRONTAB_FILE, "r+")) == NULL) {
		fprintf(stderr, "get_reservation_command: fopen error for %s\n", CRONTAB_FILE);
		return count;
	}

	while(fscanf(fp, "%[^\n]\n", reservation_command[count]) > 0)
		count++;
#ifdef DEBUG
	printf("get_reservation_command: reservation_count = %d\n", count);
#endif

	fclose(fp);
	return count;
}

/**
 * @brief 로그 파일에 이력 기록
 * @param command_type 명령 타입 번호
 * @param command 명령 문자열
 */
void write_log(int command_type, char *command) // 로그 파일에 이력 기록
{
	time_t now_t;
	struct tm *now_tm;

	pthread_mutex_lock(&mutex);
	if ((fp = fopen(CRONTAB_LOG, "r+")) == NULL)
	{
		fprintf(stderr, "write_log: fopen error for %s\n", CRONTAB_LOG);
		return;
	}
	fseek(fp, 0, SEEK_END);

	time(&now_t);
	now_tm = localtime(&now_t);

	switch (command_type) {
		case ADD:
			//sprintf(temp, "echo \"[%.24s] %s %s\" >> %s", asctime(now_tm), "add", command, CRONTAB_LOG);
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "add", command);
			break;
		case REMOVE:
			//sprintf(temp, "echo \"[%.24s] %s %s\" >> %s", asctime(now_tm), "remove", command, CRONTAB_LOG);
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "remove", command);
			break;
		case RUN:
			//sprintf(temp, "echo \"[%.24s] %s %s\" >> %s", asctime(now_tm), "run", command, CRONTAB_LOG);
			fprintf(fp, "[%.24s] %s %s\n", asctime(now_tm), "run", command);
			break;
	}
	//system(temp);
	//fflush(fp);
	fclose(fp);
	pthread_mutex_unlock(&mutex);
}


