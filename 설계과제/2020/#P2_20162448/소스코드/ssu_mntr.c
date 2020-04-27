#include "ssu_mntr.h"

int ssu_mntr(int argc, char *argv[]) // 프롬프트 메인 함수
{
	char command[MAX_BUFFER_SIZE]; // 입력받은 실행 명령 버퍼
    char *prompt = "20162448>"; // 프롬프트 기본 출력 라인
	int command_type = false; // 실행 명령 타입

    while (command_type != EXIT) {
        fputs(prompt, stdout); // 프롬프트 라인 출력, 20162448> 
		fgets(command, sizeof(command), stdin); // 실행 명령 입력
		strcpy(command, ltrim(rtrim(command))); // 실행 명령 좌우 공백 지우기
		command_type = get_command_type(command); // 명령 타입 구분
		printf("command_type : %d\n", command_type);
		// COMMANDS
		// DELETE(1)  :
		// SIZE(2)    :
		// RECOVER(3) : 
		// TREE(4)    :
		// EXIT(5)    :
		// HELP(6)    : 
		switch(command_type) {
			case DELETE:
				printf("삭제다!\n");
				break;
			case SIZE:
				printf("크기다!\n");
				break;
			case RECOVER:
				printf("복구다!\n");
				break;
			case TREE:
				printf("트리다!\n");
				break;
			case EXIT:
				printf("종료다!\n");
				break;
			case HELP:
			case UNKNOWN:
				print_usage();
				break;
			default:
				continue;
		}
    }
    fprintf(stdout, "Good bye...\n");
    fflush(stdout); // 표준 출력 스트림을 비움

    exit(0); // 프롬프트 종료
}

int get_command_type(char *command) // COMMAND 타입 확인 및 반환
{
	char *command_ptr;
	char tmp[MAX_BUFFER_SIZE] = { 0 };

	strcpy(tmp, command); // strtok 함수로 인한 버퍼 변화 방지

	if(strtok(tmp, " ") == NULL) // 명령어만 존재할 경우 확인
		command_ptr = command;
	else 
		command_ptr = tmp;

	// 명령어 타입 확인
	if(strcmp(command_ptr, "DELETE") == 0 || strcmp(command_ptr, "delete") == 0)
		return DELETE;
	else if(strcmp(command_ptr, "SIZE") == 0 || strcmp(command_ptr, "size") == 0)
		return SIZE;
	else if(strcmp(command_ptr, "RECOVER") == 0 || strcmp(command_ptr, "recover") == 0)
		return RECOVER;
	else if(strcmp(command_ptr, "TREE") == 0 || strcmp(command_ptr, "tree") == 0)
		return TREE;
	else if(strcmp(command_ptr, "EXIT") == 0 || strcmp(command_ptr, "exit") == 0)
		return EXIT;
	else if(strcmp(command_ptr, "HELP") == 0 || strcmp(command_ptr, "help") == 0)
		return HELP;
	else if(strcmp(command_ptr, "") == 0)
		return false;
	else
		return UNKNOWN;
}

char *rtrim(char *_str) // 문자열 오른쪽 공백 제거
{
	char tmp[MAX_BUFFER_SIZE];
	char *end;

	strcpy(tmp, _str);
	end = tmp + strlen(tmp) - 1;
	while(end != _str && isspace(*end))
		--end;

	*(end + 1) = '\0';
	_str = tmp;
	return _str;
}

char *ltrim(char *_str) // 문자열 왼쪽 공백 제거
{
	char *start = _str;

	while(*start != '\0' && isspace(*start))
		++start;
	_str = start;
	return _str;
}

void print_usage(void) // 사용법 출력
{
	printf("Usage : [COMMAND]\n");
	printf("COMMAND : \n");
	printf(" DELETE  [FILENAME] [END_TIME] [OPTION]     delete the file at the END_TIME\n");
	printf(" SIZE    [FILENAME] [OPTION]                print file path and size\n");
	printf(" RECOVER [FILENAME] [OPTION]                restore deleted files to their original path\n");
	printf(" TREE                                       print a list of monitoring files in a tree format.\n");
	printf(" EXIT                                       program exit\n");
	printf(" HELP                                       print usage\n");
}
