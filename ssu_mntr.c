#include "ssu_mntr.h"


void ssu_mntr(char *pwd) // 프롬프트 메인 함수
{
	char check_path[BUFFER_SIZE]; // $(PWD)/check 절대경로
	char command[MAX_BUFFER_SIZE]; // 입력받은 실행 명령 버퍼
	int command_type = false; // 실행 명령 타입
	int level_check[BUFFER_SIZE];

	sprintf(check_path, "%s/%s", pwd, CHECK);

	while (command_type != EXIT) {
		fputs(PROMPT, stdout); // 프롬프트 라인 출력, 20162448> 
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
				memset(level_check, 0, sizeof(level_check));
				file_node *head = make_list(check_path);
				print_list_tree(head, 0, level_check, true);
				//system("tree check");
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
	return;
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

void print_list_tree(file_node *head, int level, int level_check[], int is_root) // 모니터링 파일 목록 트리 출력
{
	file_node *now;
	char *file_name;
	char *tmp;

	now = head;

	while(true) {
		file_name = strtok(now->name, "/"); // 파일명 추출
		while((tmp = strtok(NULL, "/")) != NULL)
			strcpy(file_name, tmp);

		if(is_root) { // 루트 디렉토리 노드일 경우 디렉토리 이름만 출력 후 하위 파일 노드로 이동
			printf("%s\n", file_name); // 파일명 출력
			now = now->child; // 하위 파일 노드로 이동
			is_root = false; // 루트 확인 해제
			level_check[level++] = true; // 레벨 증가
			continue;
		}

		print_indent(level, level_check);

		if(now->next != NULL) 
			printf("├──%s\n", file_name);
		else
			printf("└──%s\n", file_name);

		if(S_ISDIR(now->attr.st_mode)) { // 같은 레벨의 파일이 디렉토리일 경우
			if(now->next != NULL) { // 같은 레벨의 파일들이 더 존재할 경우
				if(now->child != NULL) {
					level_check[level] = true;
					print_list_tree(now->child, level + 1, level_check, is_root);
					level_check[level] = false;
				}
			} else  // 같은 레벨의 파일들이 더 존재하지 않을 경우
				print_list_tree(now->child, level + 1, level_check, is_root);
		}

		if(now->next != NULL)
			now = now->next;

		else break;
	}	
}

void print_indent(int level, int level_check[]) // 트리 출력 보조 함수
{
	int i;

	for(i = 1; i < level; i++) {
		if(level_check[i])
			printf("│  ");
		else
			printf("   ");
	}
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
	printf("Command : \n");
	printf(" DELETE  <FILENAME> <END_TIME> [OPTION]     delete the file at the END_TIME\n");
	printf(" Option :  -i	                            delete file without moving 'trash' directory\n");
	printf("           -r                               re-confirm when the specified time, delete or not to delete\n\n");
	printf(" SIZE    <FILENAME> [OPTION]                print relative file path and size\n");
	printf(" Option :  -d  <NUMBER>                     print as much as NUMBER level\n\n");
	printf(" RECOVER <FILENAME> [OPTION]                restore deleted files to their original path\n");
	printf(" Option :  -l                               print file list in 'trash' directory sort by oldest deletion time before command execute\n\n");
	printf(" TREE                                       print a list of monitoring files in a tree format.\n");
	printf(" EXIT                                       program exit\n");
	printf(" HELP                                       print usage\n");
}
