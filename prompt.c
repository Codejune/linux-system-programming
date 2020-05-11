#include "prompt.h"

char pwd[BUFFER_SIZE];
int in_fd;
int out_fd;
int err_fd;

void prompt(void) // 프롬프트 메인 함수
{
	// 프롬프트
	char check_path[MAX_BUFFER_SIZE]; // $(PWD)/check 절대경로
	char command_line[MAX_BUFFER_SIZE]; // 입력받은 실행 명령 버퍼

	// 명령어
	commands command;
	int command_type = false; // 실행 명령 타입
	char target_path[BUFFER_SIZE]; // FINE_NAME 경로

	// 공통
	file_node *head;
	int is_invalid;
	int is_filename;
	int idx;

	// DELETE
	struct tm reserv_tm;
	time_t current_t;
	time_t reserv_t;
	int is_endtime;
	int option_i;
	int option_r;
	int sec;

	// SIZE
	int number;
	int option_d;

	// RECOVER
	int option_l;
	char trash_files_path[MAX_BUFFER_SIZE];
	char file_name[BUFFER_SIZE];

	// TREE
	int level_check[BUFFER_SIZE];

	// 사용자 정의 시그널 
	// SIGUSR1: 표준입출력 닫기
	// SIGUSR2: 표준입출력 열기
	signal(SIGUSR1, swap_handler);
	signal(SIGUSR2, swap_handler);
	getcwd(pwd, BUFFER_SIZE);
	sprintf(check_path, "%s/%s", pwd, CHECK); // 모니터링 디렉토리 경로 

	while (command_type != EXIT) {

		fputs(PROMPT, stdout);
		fgets(command_line, MAX_BUFFER_SIZE, stdin); // 실행 명령 입력
		strcpy(command_line, ltrim(rtrim(command_line))); // 실행 명령 좌우 공백 지우기
		command = make_command_token(command_line); 
		command_type = get_command_type(command.argv[0]); // 명령 타입 구분

		// COMMANDS
		// DELETE(1)  : DELETE  <FILENAME> <END_TIME> [OPTION]
		// SIZE(2)    : SIZE    <FILENAME> [OPTION]
		// RECOVER(3) : RECOVER <FILENAME> [OPTION]
		// TREE(4)    : TREE
		// EXIT(5)    : EXKT
		// HELP(6)    : HELP

		switch(command_type) {
			case DELETE: 

				is_invalid = false;
				is_endtime = false;
				is_filename = false;
				option_i = false;
				option_r = false;

				chdir(CHECK);

				if(command.argc < 2) { // 인자 개수가 부족할 경우
					fprintf(stderr, "%s1: FILE_NAME doesn't exist\n", command.argv[0]);
					continue;
				}

				// 명령행 인자 파싱 및 에러 검출 순위
				// 1. 옵션
				// 2. 파일명
				// 3. 시간
				for(idx = 1; idx < command.argc; idx++) {

					// 옵션 파싱
					if(command.argv[idx][0] == '-') {
						if(command.argv[idx][1] == 'r') // -r 옵션
							option_r = true;
						else if(command.argv[idx][1] == 'i') // -i 옵션
							option_i = true;
						else {
							fprintf(stderr, "%s: invalid option %c\n", command.argv[0], command.argv[idx][1]);
							is_invalid = true;
							break;
						}
						continue;
					}

					// FILE_NAME 파싱
					if(!is_filename) {
						if(access(command.argv[idx], F_OK) < 0) { // 파일이 존재하지 않을 경우
							fprintf(stderr, "%s: access error for %s\n", command.argv[0], command.argv[idx]);
							is_invalid = true;
							break;
						} else { // 파일이 존재하는 경우
							realpath(command.argv[idx], target_path); // 절대 경로로 변경
							if(strstr(target_path, check_path) == NULL) { // FILE_NAME이 모니터링 디렉토리에 존재하지 않을 경우
								fprintf(stderr, "%s: %s doesn't exist in %s\n", command.argv[0], command.argv[idx], check_path);
								is_invalid = true;
								break;
							}
							is_filename = true;
						}
						continue;
					}

					// END_TIME 파싱
					if(!is_endtime && (idx + 1 < command.argc)) {
						current_t = time(NULL);
						reserv_tm = get_tm(command.argv[idx], command.argv[idx + 1]);
						reserv_t = mktime(&reserv_tm);
						if((sec = (reserv_t -  current_t)) < 0) {
							fprintf(stderr, "%s: invalid END_TIME\n", command.argv[0]);
							is_invalid = true;
							break;
						}
						is_endtime = true;
					}
				}

				chdir(pwd);

				if(is_invalid) // 파싱 중 에러 발견 시
					break;
				else if(!is_filename) { // FILE_NAME이 존재하지 않을 경우
					fprintf(stderr, "%s: invalid input\n", command.argv[0]);
					break;
				} else if(option_r && !is_endtime) { // -r 옵션이 주어지고, END_TIME이 존재하지 않는 경우
					fprintf(stderr, "%s -r: END_TIME doesn't exist\n", command.argv[0]);
					break;
				}

				if(is_endtime) // END_TIME이 존재할 경우
					wait_thread(target_path, sec, option_r, option_i);
				else { // END_TIME이 존재하지 않을 경우
					head = make_list(target_path); // 파일 목록 구조체 생성
					move_trash(head, option_i);
					free_list(head);
				}

				while(check_trash_info())
					delete_trash_oldest();

				break;

			case SIZE:

				number = true;
				is_invalid = false;
				is_filename = false;
				option_d = false;

				if(command.argc < 2) { // 인자 개수가 부족할 경우
					fprintf(stderr, "%s2: FILE_NAME doesn't exist\n", command.argv[0]);
					continue;
				}

				// 명령행 인자 파싱 및 에러 검출 순위
				// 1. 옵션
				// 2. 파일명
				for(idx = 1; idx < command.argc; idx++) {

					//옵션 파싱
					if(command.argv[idx][0] == '-') {
						if(command.argv[idx][1] == 'd') { // -d 옵션
							option_d = true;
							if(idx + 1 == command.argc) {
								fprintf(stderr, "%s: NUMBER doesn't exist\n", command.argv[0]);
								is_invalid = true;
								break;
							} else if((number = atoi(command.argv[++idx])) <= 0) { // NUMBER 
								fprintf(stderr, "%s: invalid input NUMBER\n", command.argv[0]);
								is_invalid = true;
								break;
							}
						} else {
							fprintf(stderr, "%s: invalid option %c\n", command.argv[0], command.argv[idx][1]);
							is_invalid = true;
							break;
						}
						continue;
					}

					// FILE_NAME 파싱
					if(!is_filename) {
						if(access(command.argv[idx], F_OK) < 0) { // 파일이 존재하지 않을 경우
							fprintf(stderr, "%s: access error for %s\n", command.argv[0], command.argv[idx]);
							is_invalid = true;
							break;
						} else { // 파일이 존재하는 경우
							realpath(command.argv[idx], target_path); // 절대 경로로 변경
							if(strstr(target_path, check_path) == NULL) { // FILE_NAME이 모니터링 디렉토리에 존재하지 않을 경우
								fprintf(stderr, "%s: %s doesn't exist in %s\n", command.argv[0], command.argv[idx], check_path);
								is_invalid = true;
								break;
							}
							is_filename = true;
						}
						continue;
					}
				}

				if(is_invalid) // 파싱 중 에러 발견 시
					break;
				else if(!is_filename) {
					fprintf(stderr, "%s: invalid input\n", command.argv[0]);
					break;
				}

				head = make_list(target_path); // 해당 경로의 파일 목록 구조체 생성
				print_list_size(head, target_path, number, option_d, true); // 출력
				free_list(head);

				break;

			case RECOVER:

				is_filename = false;
				option_l = false;
				is_invalid = false;

				if(command.argc < 2) { // 인자 개수가 부족할 경우
					fprintf(stderr, "%s3: FILE_NAME doesn't exist\n", command.argv[0]);
					continue;
				}

				// 명령행 인자 파싱 및 에러 검출 순위
				// 1. 옵션
				// 2. 파일명
				for(idx = 1; idx < command.argc; idx++) {

					//옵션 파싱
					if(command.argv[idx][0] == '-') {
						if(command.argv[idx][1] == 'l') // -l 옵션
							option_l = true;
						else {
							fprintf(stderr, "%s: invalid option %c\n", command.argv[0], command.argv[idx][1]);
							is_invalid = true;
							break;
						}
						continue;
					}

					// FILE_NAME 파싱
					if(!is_filename) {
						sprintf(trash_files_path, "%s/%s/%s", pwd, TRASH_FILES, command.argv[idx]);
						if(access(trash_files_path, F_OK) < 0) { // 파일이 존재하지 않을 경우
							sprintf(trash_files_path, "%s/%s/1_%s", pwd, TRASH_FILES, command.argv[idx]);
							if(access(trash_files_path, F_OK) < 0) { // 중복 파일 존재 유무 확인
								fprintf(stderr, "%s: access error for %s\n", command.argv[0], command.argv[idx]);
								is_invalid = true;
								break;
							}
						}

						strcpy(file_name, command.argv[idx]);
						is_filename = true;
						continue;
					}
				}

				if(is_invalid) // 파싱 중 에러 발견 시
					break;
				else if(!is_filename) {
					fprintf(stderr, "%s: invalid input\n", command.argv[0]);
					break;
				}

				restore_file(file_name, option_l);
				break;

			case TREE:

				memset(level_check, 0, BUFFER_SIZE);
				head = make_list(check_path); // 모니터링 디렉토리 파일 목록 구조체 생성
				print_list_tree(head, 0, level_check, true); // 출력 
				free_list(head); // 메모리 할당 해제
				break;

			case HELP:
			case UNKNOWN:

				print_usage();
				break;

			case EXIT:
			default:
				break;
		}
		free_command(command);
		fflush(stdin);
		fflush(stdout);
	}
	printf("Successful prompt termination.\n");
}

commands make_command_token(char *command_line) // 명령어 전체 문장 토큰화
{
	commands result;
	char *tmp;
	char *command;

	result.argv = (char **)calloc(BUFFER_SIZE, sizeof(char*));
	result.argc = 0;

	if((command = strtok(command_line, " ")) == NULL) { // 엔터만 쳤을 경우
		result.argv[result.argc] = (char *)calloc(true, sizeof(char));
		strcpy(result.argv[result.argc], "");
		return result;
	}

	to_lower_case(command); // 명령어 소문자화
	result.argv[result.argc] = (char *)calloc(strlen(command), sizeof(char)); // 메모리 공간 할당
	strcpy(result.argv[result.argc++], command); // 토큰 배열에 복사

	while((tmp = strtok(NULL, " ")) != NULL) { // 나머지 인자 복사
		result.argv[result.argc] = (char *)calloc(strlen(command), sizeof(char)); // 메모리 공간 할당
		strcpy(result.argv[result.argc++], tmp); // 토큰 배열에 복사
	}

	return result;
}

int get_command_type(char *command) // COMMAND 타입 확인 및 반환
{
	// 명령어 타입 확인
	if(!strcmp(command,""))
		return false;
	else if(!strcmp(command, "delete"))
		return DELETE;
	else if(!strcmp(command, "size"))
		return SIZE;
	else if(!strcmp(command, "recover"))
		return RECOVER;
	else if(!strcmp(command, "tree"))
		return TREE;
	else if(!strcmp(command, "exit"))
		return EXIT;
	else if(!strcmp(command, "help"))
		return HELP;
	else
		return UNKNOWN;
}

char *get_file_name(char *path) // 파일명 추출
{
	static char file_name[BUFFER_SIZE];
	char *tmp;

	strcpy(file_name, strtok(path, "/"));
	while((tmp = strtok(NULL, "/")) != NULL) {
		strcpy(file_name, tmp);
	}
	return (char*)file_name;
}

void move_trash(file_node *head, int option_i) // 파일을 휴지통 이동
{
	FILE *fp;
	char *file_name;
	char target_path[MAX_BUFFER_SIZE];
	char trash_files_path[MAX_BUFFER_SIZE];
	char trash_info_path[MAX_BUFFER_SIZE];
	char *time_format;
	struct tm time_info;
	time_t cur_time;
	int overlap;

	sprintf(trash_files_path, "%s/%s", pwd, TRASH_FILES);
	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);
	strcpy(target_path, head->name);

	if(access(TRASH, F_OK) < 0) // 휴지통 디렉토리가 존재하지 않을 경우
		mkdir(TRASH, DIR_MODE);

	if(access(TRASH_FILES, F_OK) < 0) // 삭제 파일 저장 디렉토리가 존재하지 않을 경우
		mkdir(TRASH_FILES, DIR_MODE);

	if(access(TRASH_INFO, F_OK) < 0) // 삭제 파일 정보 디렉토리가 존재하지 않을 경우
		mkdir(TRASH_INFO, DIR_MODE);

	file_name = get_file_name(target_path); // 파일 이름 추출

	if(option_i) {
		if(S_ISDIR(head->attr.st_mode))
			remove_directory(head->name);
		else
			unlink(head->name);
	} else {
		// 파일 정보 생성
		if((overlap = find_trash_overlap(file_name)) > 0) 
			sprintf(target_path, "%s/%s/%d_%s.txt", pwd, TRASH_INFO, overlap, file_name);
		else 
			sprintf(target_path, "%s/%s/%s.txt", pwd, TRASH_INFO, file_name);

		if((fp = fopen(target_path, "w+")) < 0) {
			fprintf(stderr, "fopen error for %s\n", target_path);
			return;
		}

		time(&cur_time);
		time_info = *localtime(&cur_time);
		time_format = make_time_format(time_info);
		fprintf(fp, "[Trash info]\n%s\nD : %s\n", head->name, time_format);

		time_info = *localtime(&(head->attr.st_mtime));
		time_format = make_time_format(time_info);
		fprintf(fp, "M : %s\n", time_format);
		fclose(fp);

		// 파일 원본 이동
		if(overlap > 0)
			sprintf(target_path, "%s/%s/%d_%s", pwd, TRASH_FILES, overlap, file_name);
		else 
			sprintf(target_path, "%s/%s/%s", pwd, TRASH_FILES, file_name); // 이동할 경로 생성
		rename(head->name, target_path); // 이동
	}
}

void wait_thread(char *path, int sec, int option_r, int option_i) // 삭제 대기 스레드
{
	file_node* head;
	pid_t pid, ppid;
	char input;

	ppid = getpid();

	if((pid = fork()) < 0)
		fprintf(stderr, "delete: fork error\n");
	else if(pid == 0) {

		sleep(sec); // 시간 대기

		if(access(path, F_OK) < 0) { // 파일이 존재하지 않을 경우
			fprintf(stderr, "delete: access error for %s\n", path);
			return;
		}

		head = make_list(path); // 파일 목록 구조체 생성
		if(option_r) { // -r옵션이 존재할 경우
			kill(ppid, SIGUSR1); // 부모 프로세스의 표준 입출력 닫기
			printf("\nDelete [y/n]? ");
			input = fgetc(stdin);
			switch(input) {
				case 'y':
					move_trash(head, option_i);
					printf("please input ENTER to continue...");
					break;
				case 'n':
					break;
				default:
					fputs("delete: invalid input, thread was being aborted\n", stdout);
					break;
			}
		} else 
			move_trash(head, option_i);
		free_list(head);
		kill(ppid, SIGUSR2); // 부모 프로세스의 표준 입출력 열기
		exit(0); // 자식 프로세스 종료
	}
	return; // 부모 프로세스 반환
}

void swap_handler(int signo){ // 시그널로 표준입출력 전환
	switch(signo) {
		case SIGUSR1:
			in_fd = dup(0);
			out_fd = dup(1);
			err_fd = dup(2);
			close(0);
			close(1);
			close(2);
			pause();
			break;
		case SIGUSR2:
			dup2(in_fd, 0);
			dup2(out_fd, 1);
			dup2(err_fd, 2);
			break;
	}
}

struct tm get_tm(char *date, char *time) // 시간 구조체 획득
{
	static struct tm tmp;
	int year, month, day;
	int hour, min, sec;

	year = month = day = hour = min = sec = 0;

	sscanf(date, "%d-%d-%d", &year, &month, &day);
	sscanf(time, "%d:%d:%d", &hour, &min, &sec);

	// 시간 구조체 파싱 예외
	if(year < 0 || month < 0 || month > 12 || day < 0 || day > 31 ||  hour < 0 || hour > 24 || min < 0 || min > 60 || sec < 0 || sec > 60)
		return tmp;
	else {
		tmp.tm_year = year - 1900;
		tmp.tm_mon = month - 1;
		tmp.tm_mday = day;
		tmp.tm_hour = hour;
		tmp.tm_min = min;
		tmp.tm_sec = sec;
	}

	return tmp;
}

void remove_directory(const char *path) // 디렉토리 삭제
{
	// trash와 info에 저장하는 코드 작성 필요!
	file_node *now;
	struct dirent *dirp;
	struct stat statbuf;
	DIR *dp;
	char tmp[MAX_BUFFER_SIZE];

	if((dp = opendir(path)) == NULL)
		return;

	while((dirp = readdir(dp)) != NULL) { // path에 존재하는 디렉토리 안에 파일들 전부 삭제
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name); // tmp = 디렉토리 내부 파일

		if(lstat(tmp, &statbuf) == -1) // 파일 상태 정보 추출
			continue;

		if(S_ISDIR(statbuf.st_mode)) // 디렉토리일 경우 재귀적으로 제거
			remove_directory(tmp);
		else 
			unlink(tmp);
	}
	closedir(dp);
	rmdir(path);
}

int check_trash_info(void) // 휴지통 파일 정보 디렉토리 크기 확인
{	
	char trash_info_path[MAX_BUFFER_SIZE];
	file_node *head;
	int size;

	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);
	head = make_list(trash_info_path);
	size = head->size;
	free_list(head);

	if(size > MAX_INFO_SIZE)
		return true;
	return false;
}

void delete_trash_oldest(void) // 휴지통에서 가장 오래 삭제된 파일 제거
{ 
	char trash_info_path[MAX_BUFFER_SIZE];
	char old_info[BUFFER_SIZE];
	char old_path[MAX_BUFFER_SIZE]; // 가장 오래된 파일 경로
	char old_name[BUFFER_SIZE];
	char date[BUFFER_SIZE];
	char time[BUFFER_SIZE];
	char path[MAX_BUFFER_SIZE];
	char tmp[MAX_BUFFER_SIZE];
	struct tm tmp_tm;
	FILE *fp;
	time_t old_sec; // 가장 오래된 파일 삭제 시간
	time_t now_sec;
	int file_count;
	struct stat statbuf;
	struct dirent **namelist;
	int is_first;
	int i;

	is_first = true;
	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);
	chdir(trash_info_path);

	file_count = scandir(trash_info_path, &namelist, NULL, alphasort);
	for(i = 0; i < file_count; i++) { // 정보 파일이 저장된 디렉토리 탐색

		if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..")) 
			continue;

		sprintf(tmp, "%s/%s/%s", pwd, TRASH_INFO, namelist[i]->d_name); // 정보 파일 경로

		// 정보 파일에서 데이터 추출
		if((fp = fopen(tmp, "r+")) < 0) { // 파일 읽기 모드로 열기
			fprintf(stderr, "fopen error for %s\n", namelist[i]->d_name);
			continue;
		}
		fseek(fp, 13, SEEK_SET); // 헤더 생략
		fscanf(fp, "%s\n", path); // 파일 경로
		fscanf(fp, "D : %s %s\n", date, time); // 삭제 시간
		fclose(fp); // 파일 닫기

		// 시간 변환
		tmp_tm = get_tm(date, time);
		now_sec = mktime(&tmp_tm);

		// 경로 변환
		sprintf(path, "%s/%s/%s", pwd, TRASH_FILES, get_file_name(path));

		if(is_first) { // 최초 탐색일 경우
			old_sec = now_sec; 
			strcpy(old_info, tmp); 
			strcpy(old_name, namelist[i]->d_name);
			is_first = false;
		} else {
			if(now_sec < old_sec) {
				old_sec = now_sec;
				strcpy(old_info, tmp);
				strcpy(old_name, namelist[i]->d_name);
			}
		}
	}

	sprintf(old_path, "%s/%s/", pwd, TRASH_FILES);
	strncat(old_path, old_name, strlen(old_name) - 4);

	stat(old_path, &statbuf); // 가장 오래된 파일 정보 획득

	if(S_ISDIR(statbuf.st_mode)) // 디렉토리일 경우
		remove_directory(old_path);
	else // 파일일 경우
		unlink(old_path);
	unlink(old_info);
	chdir(pwd);
	free(namelist);
}

int find_trash_overlap(const char *file_name) // 휴지통 중복 파일 탐색
{
	char trash_info_path[MAX_BUFFER_SIZE];
	char trash_files_path[MAX_BUFFER_SIZE];
	char target_path[MAX_BUFFER_SIZE];
	struct dirent **namelist;
	int overlap_count;
	int file_count;
	int tmp;
	int i;

	overlap_count = 0;
	sprintf(trash_files_path, "%s/%s", pwd, TRASH_FILES);
	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);

	chdir(trash_files_path);

	file_count = scandir(trash_files_path, &namelist, NULL, alphasort);
	for(i = 0; i < file_count; i++) {

		if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..")) 
			continue;

		if(!strcmp(file_name, namelist[i]->d_name)) { // 최초 중복되는 경우
			sprintf(target_path, "%s/%s/1_%s", pwd, TRASH_FILES, file_name);
			sprintf(trash_files_path, "%s/%s/%s", pwd, TRASH_FILES, file_name);
			rename(trash_files_path, target_path); // 원본 파일 이름 변경 
			sprintf(target_path, "%s/%s/1_%s.txt", pwd, TRASH_INFO, file_name);
			sprintf(trash_info_path, "%s/%s/%s.txt", pwd, TRASH_INFO, file_name);
			rename(trash_info_path, target_path); // 정보 파일 이름 변경
			chdir(pwd);
			free(namelist);
			return true + 1;
		}

		sscanf(namelist[i]->d_name, "%d_%s", &tmp, target_path);
		if(tmp > false && !strcmp(target_path, file_name)) // 다수 중복되는 경우
			overlap_count++; 
	}

	free(namelist);

	if(overlap_count > 0) {
		chdir(pwd);
		return overlap_count + 1;
	}

	chdir(pwd);
	return false;
}

void print_list_size(file_node *head, char *path, int number, int option_d, int op_switch) // 지정 파일 상대 경로 및 크기 출력
{
	char *relative_path;
	file_node *now;

	now = head;

	while(number > 0) {

		if(now == NULL) // 파일이 존재하지 않을 경우
			break;

		relative_path = now->name + strlen(pwd); // 상대 경로 추출
		printf("%-10d.%-s\n", now->size, relative_path); // 출력

		if(op_switch) {
			if(option_d) { // 옵션이 주어졌을 경우
				if(!S_ISDIR(now->attr.st_mode))  // 파일인 경우
					break; // 탐색 종료
			} else break; // 옵션이 주어지지 않은 경우
		}

		op_switch = false;

		if(now->child != NULL) // 하위 디렉토리 파일들이 존재하면 
			print_list_size(now->child, path, number - 1, option_d, op_switch); // 하위 디렉토리 파일 출력 

		now = now->next; // 같은 레벨의 파일들 이어서 탐색
	}
}

void restore_file(const char *file_name, int option_l) // 휴지통 파일 복원
{
	char trash_info_path[MAX_BUFFER_SIZE];
	char trash_files_path[MAX_BUFFER_SIZE];
	char tmp[MAX_BUFFER_SIZE];
	char date[BUFFER_SIZE];
	char time[BUFFER_SIZE];
	char *temp;
	struct dirent **namelist;
	int file_count;
	FILE *fp;
	file_infos file_info[50];
	int idx;
	int overlap;
	int i, j;

	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);
	sprintf(trash_files_path, "%s/%s", pwd, TRASH_FILES);

	if(option_l) { // -l 옵션

		idx = 0;

		file_count = scandir(trash_info_path, &namelist, NULL, alphasort); // 파일 정보 디렉토리 탐색
		for(i = 0; i < file_count; i++) {

			if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..")) // 상위 디렉토리 접근자 생략
				continue;

			sprintf(tmp, "%s/%s/%s", pwd, TRASH_INFO, namelist[i]->d_name); // 정보 파일 경로 생성

			if((fp = fopen(tmp, "r+")) < 0) { // 정보 파일 읽기 모드로 열기
				fprintf(stderr, "fopen error for %s\n", namelist[i]->d_name);
				continue;
			}
			fseek(fp, 13, SEEK_SET); // 정보 파일 제목 생략

			// 파일 경로 추출
			fscanf(fp, "%s\n", file_info[idx].path);

			// 파일 삭제 시간 추출
			fscanf(fp, "D : %s %s\n", date, time); // 삭제 시간 
			file_info[idx++].d_tm = get_tm(date, time);

			fclose(fp); // 파일 닫기
		} 

		free(namelist); // 메모리 할당 해제
		sort_info_oldest(file_info, idx); // 구조체 배열 삭제 시간 순 오름차순 정렬

		for(i = 0; i < idx; i++)  // 출력
			printf("%d. %-10s %s\n", i + 1, get_file_name(file_info[i].path), make_time_format(file_info[i].d_tm)); 
	} 

	// 파일 복원 우선 순위
	// 1. 휴지통 디렉토리에 존재하는 경우
	// 2. 휴지통 디렉토리에 중복으로 존재하는 경우
	idx = 0;

	file_count = scandir(trash_info_path, &namelist, NULL, alphasort); // 파일 정보 디렉토리 탐색
	for(i = 0; i < file_count; i++) { 

		if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..")) // 상위 디렉토리 접근자 생략
			continue;

		memset(tmp, 0, MAX_BUFFER_SIZE);
		strncpy(tmp, namelist[i]->d_name, strlen(namelist[i]->d_name) - 4); // 정보 파일명에서 원본 파일명 추출

		// 1. 휴지통에 해당 파일이 존재하는 경우
		if(!strcmp(file_name, tmp)) { 

			sprintf(tmp, "%s/%s/%s", pwd, TRASH_INFO, namelist[i]->d_name); // 정보 파일 경로 생성
			if((fp = fopen(tmp, "r+")) < 0) { // 정보 파일 읽기 모드로 열기
				fprintf(stderr, "fopen error for %s\n", namelist[i]->d_name);
				continue;
			}
			fseek(fp, 13, SEEK_SET); // 정보 파일 제목 생략

			// 파일 경로 추출
			fscanf(fp, "%s\n", file_info[idx].path); 

			fclose(fp); // 파일 닫기

			temp = get_file_path(file_info[idx].path, file_name);
			if(access(temp, F_OK) < 0) { // 복원 지점까지의 경로가 존재하지 않을 경우
				fprintf(stderr, "recover: %s path doesn't exist\n", temp);
				return;
			}

			if(access(file_info[idx].path, F_OK) < 0) { // a. 복원 지점에 똑같은 이름의 파일이 존재하지 않을 경우

				// 정보 파일 삭제 
				unlink(tmp); 

				// 원본 파일 복원
				sprintf(tmp, "%s/%s/%s", pwd, TRASH_FILES, file_name); // 휴지통 원본 파일 경로 생성
				rename(tmp, file_info[idx].path); // 휴지통 원본 파일을 복원 지점으로 이동

			} else { // b. 복원 지점에 똑같은 이름의 파일이 존재하는 경우

				j = 1; // 중복 파일 카운트

				temp = get_file_path(file_info[idx].path, file_name); // 복원 지점 경로 추출, path/

				while(true) { 
					sprintf(tmp, "%s%d_%s", temp, j, file_name); // 복원 파일 이름 생성
					if(access(tmp, F_OK) < 0) { // 복원 지점에 해당 이름의 파일이 존재하지 않는 경우

						// 정보 파일 삭제
						sprintf(trash_info_path, "%s/%s/%s.txt", pwd, TRASH_INFO, file_name); // 휴지통 정보 파일 경로 생성
						unlink(trash_info_path);

						// 원본 파일 복원
						sprintf(trash_files_path, "%s/%s/%s", pwd, TRASH_FILES, file_name); // 휴지통 원본 파일 경로 생성
						rename(trash_files_path, tmp); // 휴지통 원본 파일을 복원지점으로 이동

						break;
					}
					j++; // 중복 파일 카운트 증가
				}
			}
			free(namelist); // 메모리 할당 해제
			return; 
		}

		memset(tmp, 0, MAX_BUFFER_SIZE);
		strncpy(tmp, namelist[i]->d_name, strlen(namelist[i]->d_name) - 4);
		sscanf(tmp, "%d_%s", &overlap, tmp); // 중복 파일 카운트, 파일 이름 추출 

		// 2. 휴지통에 해당 파일이 중복으로 존재하는 경우
		if(!strcmp(file_name, tmp)) { 

			sprintf(tmp, "%s/%s/%s", pwd, TRASH_INFO, namelist[i]->d_name); // 정보 파일 경로 생성

			if((fp = fopen(tmp, "r+")) < 0) { // 정보 파일 읽기 모드로 열기
				fprintf(stderr, "fopen error for %s\n", namelist[i]->d_name);
				continue;
			}
			fseek(fp, 13, SEEK_SET); // 정보 파일 제목 생략

			file_info[idx].num = overlap; // 중복 파일 카운트 저장

			// 파일 경로 추출
			fscanf(fp, "%s\n", file_info[idx].path);

			// 파일 시간 정보 추출
			fscanf(fp, "D : %s %s\n", date, time); // 삭제 시간 
			file_info[idx].d_tm = get_tm(date, time);
			fscanf(fp, "M : %s %s\n", date, time); // 수정 시간
			file_info[idx].m_tm = get_tm(date, time);

			fclose(fp); // 파일 닫기

			idx++; // 구조체 카운트 증가
		}

	} // 휴지통 탐색 완료

	free(namelist); // 메모리 할당 해제
	sort_info_order(file_info, idx); // 구조체 배열 중복 파일 카운트순 오름차순 정렬 

	// 중복된 파일 중 선택 복원
	for(i = 0; i < idx; i++)
		printf("%d. %-10s D : %s M : %s\n", i + 1, file_name, make_time_format(file_info[i].d_tm), make_time_format(file_info[i].m_tm)); 
	printf("Choose : ");
	scanf("%d", &i);
	getchar();

	if(i < 1 || i >= idx) {
		fprintf(stderr, "recover: invalid input NUMBER\n");
		return;
	}

	temp = get_file_path(file_info[i].path, file_name);
	if(access(temp, F_OK) < 0) { // 복원 지점까지의 경로가 존재하지 않을 경우
		fprintf(stderr, "recover: %s path doesn't exist\n", temp);
		return;
	}

	if(access(file_info[i].path, F_OK) < 0) { // c. 복원 지점에 똑같은 이름의 파일이 존재하지 않는 경우

		// 정보 파일 삭제 
		sprintf(tmp, "%s/%s/%d_%s.txt", pwd, TRASH_INFO, i, file_name); // 휴지통 정보 파일 경로 생성
		unlink(tmp); 

		// 원본 파일 복구
		sprintf(tmp, "%s/%s/%d_%s", pwd, TRASH_FILES, i, file_name); // 휴지통 원본 파일 경로 생성
		rename(tmp, file_info[i - 1].path); // 휴지통 원본 파일을 복원 지점으로 이동

	} else { // d. 복원 지점에 똑같은 이름의 파일이 존재하는 경우

		j = 1; // 중복 파일 카운트 

		temp = get_file_path(file_info[i - 1].path, file_name); // 복원 지점 경로 생성, path/

		while(true) { 
			sprintf(tmp, "%s%d_%s", temp, j, file_name); // 복원 파일 이름 생성
			if(access(tmp, F_OK) < 0) { // 복원 지점에 해당 이름의 파일이 존재하지 않는 경우

				// 정보 파일 삭제 
				sprintf(trash_info_path, "%s/%s/%d_%s.txt", pwd, TRASH_INFO, i, file_name);
				unlink(trash_info_path);

				// 원본 파일 복원
				sprintf(trash_files_path, "%s/%s/%d_%s", pwd, TRASH_FILES, i, file_name);
				rename(trash_files_path, tmp);

				break;
			}
			j++; // 중복 파일 카운트 증가
		}

	}
	refresh_trash(file_name, idx - 1, i); // 휴지통 중복 파일 재정리
}

char *get_file_path(char *path, const char *file_name) // 파일 경로 추출
{
	char *ptr;
	static char file_path[BUFFER_SIZE];
	char *tmp;

	strcpy(file_path, path);
	ptr = strstr(file_path, file_name);
	while(ptr != NULL) {
		tmp = ptr;
		ptr = strstr(ptr+1, file_name);
	}
	memset(file_path, 0, BUFFER_SIZE);
	strncpy(file_path, path, tmp - file_path);

	return (char*)file_path;
}

void sort_info_oldest(file_infos *file_info, int idx) // 중복 파일 구조체 삭제 시간 오름차순 정렬
{
	int i, j;
	char tmp_path[BUFFER_SIZE];
	struct tm tmp_tm;

	for(i = 0; i < idx; i++)
		for(j = i + 1; j < idx; j++)
			if(mktime(&file_info[i].d_tm) > mktime(&file_info[j].d_tm)) {
				strcpy(tmp_path, file_info[i].path);
				strcpy(file_info[i].path, file_info[j].path);
				strcpy(file_info[j].path, tmp_path);

				tmp_tm = file_info[i].d_tm;
				file_info[i].d_tm = file_info[j].d_tm;
				file_info[j].d_tm = tmp_tm;
			}
}

void sort_info_order(file_infos *file_info, int idx) // 파일정보 구조체 파일 번호 오름차순 정렬
{
	int i, j;
	int tmp_num;
	char tmp_path[BUFFER_SIZE];
	struct tm tmp_tm;

	for(i = 0; i < idx; i++)
		for(j = i + 1; j < idx; j++)
			if(file_info[i].num > file_info[j].num) {
				strcpy(tmp_path, file_info[i].path);
				strcpy(file_info[i].path, file_info[j].path);
				strcpy(file_info[j].path, tmp_path);

				tmp_tm = file_info[i].d_tm;
				file_info[i].d_tm = file_info[j].d_tm;
				file_info[j].d_tm = tmp_tm;

				tmp_num = file_info[i].num;
				file_info[i].num = file_info[j].num;
				file_info[j].num = tmp_num;
			}
}


void refresh_trash(const char *file_name, int idx, int delete_idx) // 삭제 후 중복 파일 번호 재정렬 
{
	char trash_info_path[MAX_BUFFER_SIZE];
	char trash_files_path[MAX_BUFFER_SIZE];
	char tmp1[BUFFER_SIZE];
	char tmp2[BUFFER_SIZE];
	struct dirent **namelist;
	int file_count;
	int overlap;
	int i;

	sprintf(trash_info_path, "%s/%s", pwd, TRASH_INFO);
	sprintf(trash_files_path, "%s/%s", pwd, TRASH_FILES);

	// 중복 파일이 더이상 존재하지 않는 경우 
	if(idx == 1 && delete_idx == 1) { // 중복 파일 2개중 1번을 삭제한 경우
		chdir(TRASH_INFO);
		sprintf(tmp1, "2_%s.txt", file_name);
		sprintf(tmp2, "%s.txt", file_name);
		rename(tmp1, tmp2);
		chdir(pwd);
		chdir(TRASH_FILES);
		sprintf(tmp1, "2_%s", file_name);
		rename(tmp1, file_name);
		chdir(pwd);
		return;
	} else if(idx == 1 && delete_idx == 2) { // 중복 파일 2개중 2번을 삭제한 경우
		chdir(TRASH_INFO);
		sprintf(tmp1, "1_%s.txt", file_name);
		sprintf(tmp2, "%s.txt", file_name);
		rename(tmp1, tmp2);
		chdir(pwd);
		chdir(TRASH_FILES);
		sprintf(tmp1, "1_%s", file_name);
		rename(tmp1, file_name);
		chdir(pwd);
		return;
	} else { // 중복 파일 3개 이상
		file_count = scandir(trash_info_path, &namelist, NULL, alphasort);
		for(i = 0; i < file_count; i++) {

			if(!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
				continue;

			memset(tmp1, 0, BUFFER_SIZE);
			strncpy(tmp1, namelist[i]->d_name, strlen(namelist[i]->d_name) - 4);
			sscanf(tmp1, "%d_%s", &overlap, tmp1); // 중복 파일 카운트, 파일 이름 추출 

			if(overlap >= delete_idx && !strcmp(tmp1, file_name)) {
				chdir(TRASH_INFO);
				sprintf(tmp1, "%d_%s.txt", overlap, file_name);
				sprintf(tmp2, "%d_%s.txt", overlap - 1, file_name);
				rename(tmp1, tmp2);
				chdir(pwd);
				chdir(TRASH_FILES);
				sprintf(tmp1, "%d_%s", overlap, file_name);
				sprintf(tmp2, "%d_%s", overlap - 1, file_name);
				rename(tmp1, tmp2);
				chdir(pwd);
			}
		}
		free(namelist);
	}
}

void print_list_tree(file_node *head, int level, int level_check[], int is_root) // 모니터링 파일 목록 트리 출력
{
	file_node *now;
	char *file_name;

	now = head;

	while(true) {
		file_name = get_file_name(now->name);

		if(is_root) { // 루트 디렉토리 노드일 경우 디렉토리 이름만 출력 후 하위 파일 노드로 이동
			printf("%s\n", file_name); // 파일명 출력

			if(now->child != NULL) // 모니터링 디렉토리 하위에 파일이 존재하는 경우
				now = now->child; // 하위 파일 노드로 이동
			else  // 모니터링 디렉토리 하위에 파일이 존재하지 않는 경우
				break; 

			is_root = false; // 루트 확인 해제
			level_check[level++] = true; // 레벨 증가
			continue;
		}

		print_tree_indent(level, level_check);

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
				if(now->child != NULL) 
					print_list_tree(now->child, level + 1, level_check, is_root);
		}

		if(now->next != NULL)
			now = now->next;

		else break;
	}	
}

void print_tree_indent(int level, int level_check[]) // 트리 출력 보조 함수
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

void to_lower_case(char *str) // 문자열 소문자 변환
{
	int i = 0;

	while(str[i]) {
		if(str[i] >= 'A' && str[i] <= 'Z'){
			str[i] = str[i]+32;
		}
		i++;
	}
}

void free_command(commands command) // 명령행 구조체 초기화
{
	int i;

	for(i = 0; i < command.argc; i++)
		free(command.argv[i]);
	free(command.argv);
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
