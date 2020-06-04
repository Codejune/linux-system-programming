/**
 * @file ssu_rsync.c
 * @brief ssu_rsync 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_rsync.h"

// 경로
char pwd[MAX_BUFFER_SIZE] = { 0 }; // 프로그램 실행 경로
char src_path[MAX_BUFFER_SIZE] = { 0 }; // 타겟 경로
char dst_path[MAX_BUFFER_SIZE] = { 0 }; // 동기화 경로
char swap_path[MAX_BUFFER_SIZE] = { 0 }; // 스왑 파일

// 옵션
bool option_r = false; // R 옵션
bool option_t = false; // T 옵션
bool option_m = false; // M 옵션
bool is_complete = false; // 동기화 완료 확인

file_node change_list[MAX_BUFFER_SIZE]; // 변경 목록
char **saved_argv;
int saved_argc;
bool src_is_dir = false;


/**
 * @brief ssu_rsync 메인 함수
 * @param argc 인자 개수
 * @param argv 인자 문자열
 */
int main(int argc, char *argv[])
{
	// 실행 시간 측정 
	struct timeval begin_t, end_t;

	// swap 생성
	char swap_path[BUFFER_SIZE];
	char command[MAX_BUFFER_SIZE];

	// 유효 검사
	char opt;
	struct stat statbuf;
	bool is_invalid = false;
	bool is_src = false;
	bool is_dst = false;

	gettimeofday(&begin_t, NULL); // 측정 시작

	if (argc < 3) { // 인자 개수가 부족할 경우
		print_usage(argv[0]);
		exit(1);
	}

	getcwd(pwd, MAX_BUFFER_SIZE);
	copy_argument(argc, argv);

	for (int i = 1; i < argc; i++) {
#ifdef DEBUG
		printf("ssu_rsync(): argv[%d] = %s\n", i, argv[i]);
#endif
		// 옵션 생략
		if (argv[i][0] == '-') 
			continue;

		// 타겟 경로 파싱
		if (!is_src) {
			if (access(argv[i], F_OK) < 0) {
#ifdef DEBUG
				fprintf(stderr, "ssu_rsync(): access error for %s\n", argv[i]);
#endif
				is_invalid = true;
				break;
			}

			realpath(argv[i], src_path); // 절대 경로로 변환
#ifdef DEBUG
			printf("ssu_rsync(): src_path = %s\n", src_path);
#endif
			lstat(src_path, &statbuf); 
			if (S_ISDIR(statbuf.st_mode))
				src_is_dir = true;
#ifdef DEBUG
			fprintf(stderr, "ssu_rsync(): dst_path doesn't directory\n");
#endif
			is_src = true;
			continue;
		}

		// 동기화 경로 파싱
		if (!is_dst) {
			if (access(argv[i], F_OK) < 0) {
#ifdef DEBUG
				fprintf(stderr, "ssu_rsync(): access error for %s\n", argv[i]);
#endif
				is_invalid = true;
				break;
			}

			realpath(argv[i], dst_path); // 동기화 디렉토리 절대 경로 변환
#ifdef DEBUG
			printf("ssu_rsync(): dst_path = %s\n", dst_path);
#endif
			lstat(dst_path, &statbuf); // 동기화 디렉토리 상태 정보 획득
			if (!S_ISDIR(statbuf.st_mode)) { // 동기화 경로가 디렉토리가 아닐 경우
#ifdef DEBUG
				fprintf(stderr, "ssu_rsync(): dst_path doesn't directory\n");
#endif
				is_invalid = true;
				break;
			}
			is_dst = true;
			continue;
		}
	}

	// 옵션 파싱
	while ((opt = getopt(argc, argv, "rtm")) != -1) {
		switch (opt) {
			case 'r':
#ifdef DEBUG
				printf("ssu_rsync(): R option found\n");
#endif
				option_r = true;
				break;
			case 't':
#ifdef DEBUG
				printf("ssu_rsync(): T option found\n");
#endif
				option_t = true;
				break;
			case 'm':
#ifdef DEBUG
				printf("ssu_rsync(): M option found\n");
#endif
				option_m = true;
				break;
			default:
#ifdef DEBUG
				printf("ssu_rsync(): invalid option found\n");
#endif
				is_invalid = true;
				break;
		}
	}

	// 파싱 중 에러 발견
	if (is_invalid || !is_src || !is_dst) {
		print_usage(argv[0]);
		exit(1);
	}

	strncpy(swap_path, dst_path, get_file_name(dst_path) - dst_path);
#ifdef DEBUG
	printf("ssu_rsync(): cd %s\n", swap_path);
#endif

	chdir(swap_path);

#ifdef DEBUG
	sprintf(command, "tar -cvf %s.swp %s", get_file_name(dst_path), get_file_name(dst_path)); // 명령어 생성
	printf("ssu_rsync(): command = %s\n", command);
#else
	sprintf(command, "tar -cf %s.swp %s", get_file_name(dst_path), get_file_name(dst_path)); // 명령어 생성
#endif

	system(command); // 명령어 실행(압축)
	chdir(pwd); // 실행 경로로 복귀
	signal(SIGINT, recovery); // SIGINT 시그널 처리
	syncronize(src_path, dst_path); // 동기화
	sprintf(swap_path, "%.*s.swp", (int)strlen(dst_path), dst_path);
#ifdef DEBUG
	printf("ssu_rsync(): swap_path = %s\n", swap_path);
#endif
	unlink(swap_path); // swap 파일 삭제

	gettimeofday(&end_t, NULL); // 측정 종료
	ssu_runtime(&begin_t, &end_t); // 실행 시간 출력
	exit(0);
}

/**
 * @brief 명령행 인자 백업 
 * @param argc 인자 개수
 * @param argv 인자 문자열 배열
 */
void copy_argument(int argc, char *argv[]) // 명령행 인자 백업
{
	saved_argc = argc;
	saved_argv = calloc(argc, sizeof(char*));
	for(int i = 0; i < saved_argc; i++) {
		saved_argv[i] = calloc(MAX_BUFFER_SIZE, sizeof(char));
		strcpy(saved_argv[i], argv[i]);
	}
}

/**
 * @brief 동기화 함수
 * @param src_path 타겟 경로
 * @param dst_path 동기화 경로
 */
void syncronize(char *src_path, char *dst_path) // 동기화 함수
{
	file_node *src_list; // 타겟 경로 파일 목록
	file_node *dst_list; // 동기화 경로 파일 목록
	bool is_directory = false;
	int change_count = 0;

	src_list = make_list(src_path); 
	dst_list = make_list(dst_path);

	if (S_ISDIR(src_list->attr.st_mode)) { // 타겟이 디렉토리일 경우
		compare_list(src_list->child, dst_list->child);
		is_directory = true;
	} else // 타겟이 파일일 경우
		compare_file(src_list, dst_list->child);

	if (is_directory)
		change_count = write_change_list(src_list->child, change_count, CREATE, true); // 생성 혹은 수정된 파일 확인
	else 
		change_count = write_change_list(src_list, change_count, CREATE, true); // 생성 혹은 수정된 파일 확인

	if (option_m) 
		change_count = write_change_list(dst_list->child, change_count, DELETE, true); // 삭제 혹은 수정된 파일 확인

	if (option_t)
		renewal_tar(change_count);
	else
		renewal(change_count);

	free_list(src_list);
	free_list(dst_list);
	is_complete = true;
}

/**
 * @brief 노드 생성
 * @return 새로운 노드
 */
file_node *make_node(void) // 노드 생성
{
	file_node *tmp = calloc(true, sizeof(file_node));

	memset(tmp->name, 0, BUFFER_SIZE);
	tmp->next = NULL;
	tmp->child = NULL;
	tmp->namelist = NULL;
	tmp->size = 0;
	tmp->status = UNCHCK;

	return tmp;
}

/**
 * @brief 디렉토리 파일 목록 트리화
 * @param path 디렉토리 경로
 * @return 트리 루트 노드
 */
file_node *make_list(char *path) // 디렉토리 파일 목록 트리화
{
	file_node *head, *now;
	// 파일    : 노드 생성(절대경로/이름, 상태정보)
	// 디렉토리: 트리 생성
	int file_count;
	int is_dirattr = true;
	int i;

	// 부모: 현재 경로, 디렉토리 상태정보, 파일 목록 정보
	// 자식: 절대경로 파일이름, 파일 상태정보, 다음 파일 포인터 및 디렉토리 하위 부모노드 포인터 
	head = make_node(); 
	now = head;

	strcpy(head->name, path); // 현재 경로 저장
	stat(head->name, &(head->attr)); // 상태 정보 저장

	file_count = scandir(head->name, &(head->namelist), NULL, alphasort); // 현재 경로의 모든 파일 탐색 및 개수 저장
	for(i = 0; i < file_count; i++) {

		if(!strcmp(head->namelist[i]->d_name, ".") || !strcmp(head->namelist[i]->d_name, "..")) // 현재, 상위 디렉토리 접근 지정자 생략
			continue;

		file_node *new = make_node(); // 새로운 노드 생성

		sprintf(new->name, "%s/%s", head->name, head->namelist[i]->d_name); // 파일 이름 저장
		stat(new->name, &(new->attr));

		if(S_ISDIR(new->attr.st_mode)) // 현재 경로의 파일 목록 중 탐색한 파일이 디렉토리일 경우
			new = make_list(new->name); // 해당 디렉토리 파일 목록 트리화
		else 
			new->size = new->attr.st_size;

		if(is_dirattr) { // 현재 노드가 현재 경로의 부모노드일 경우
			now->child = new;
			now = now->child;
			is_dirattr = false;
		} else { // 아닐 경우 형제로 연결
			now->next = new;
			now = now->next;
		}
	}
	head->size = count_size(head);
	return head;
}

/**
 * @brief 디렉토리 크기 반환
 * @param 디렉토리 노드
 * @return 디렉토리 크기
 */
int count_size(file_node *head) // 디렉토리 크기 반환
{
	file_node *now;
	int size;

	size = false;

	if(S_ISDIR(head->attr.st_mode))
		now = head->child;
	else 
		return head->attr.st_size;

	while(now != NULL) {
		size += now->size;
		now = now->next;
	}

	return size;
}

/**
 * @brief 파일 목록 트리 비교 
 * @param src_list 타겟 파일 목록
 * @param dst_list 동기화 디렉토리 파일 목록 
 */
void compare_list(file_node *src_list, file_node *dst_list) // 파일 목록 트리 비교
{
	file_node *now;

	if (src_list == NULL || dst_list == NULL) { // 둘중 하나라도 비교 대상이 존재하지 않을 경우
#ifdef DEBUG
		printf("compare_list(): there is NULL node exist\n");
#endif
		return;
	}

	now = src_list;

	while (now != NULL) { // 타겟 파일 탐색

		compare_file(now, dst_list);

		if (now->child != NULL)
			compare_list(now->child, dst_list);

		now = now->next;
	}
}

/**
 * @brief 파일 정보 비교
 * @param src_file 타겟 파일 노드
 * @param dst_file 동기화 디렉토리 파일 노드
 * @return 비교 성공 유무
 */
bool compare_file(file_node *src_file, file_node *dst_file) // 파일 정보 비교
{
	file_node *now;

	now = dst_file;

	while (now != NULL) {

#ifdef DEBUG
		printf("compare_file(): src_file->name = %s, dst_file->name = %s\n", src_file->name + strlen(src_path) - strlen(get_file_name(src_path)), now->name + strlen(dst_path) + 1);
#endif
		if (!strcmp(src_file->name + strlen(src_path) - strlen(get_file_name(src_path)), now->name + strlen(dst_path) + 1)) { // 파일 이름이 같은 경우

#ifdef DEBUG
			printf("compare_file(): file found\n");
#endif
			src_file->status = CHCKED;

			if (src_file->attr.st_mode != now->attr.st_mode) { // 1. 파일 형식이 다를 경우
#ifdef DEBUG
				printf("compare_file(): type different\n");
#endif
				src_file->status = MODIFY;
			} else if (src_file->attr.st_mtime != now->attr.st_mtime) { // 2. 수정시간이 다를 경우
#ifdef DEBUG
				printf("compare_file(): mtime different\n");
#endif
				src_file->status = MODIFY;
			} else if (src_file->size != now->size) { // 3. 크기가 다를 경우

#ifdef DEBUG
				printf("compare_file(): size different\n");
#endif
				src_file->status = MODIFY;
			}

			now->status = CHCKED;
#ifdef DEBUG
			printf("compare_file(): src_file->status = %d, dst_file->status = %d\n", src_file->status, now->status);
#endif
			return true;
		}

		if(now->child != NULL) // 디렉토리 안에 파일이 존재할 경우
			if(compare_file(src_file, now->child)) 
				break;

		now = now->next;
	}

	return false;
}

/**
 * @brief 변경사항 목록 작성
 * @param head 트리 루트 노드
 * @param idx 변경사항 목록 시작 인덱스
 * @param status 변경 사항 타입 번호
 * @param is_first 첫번째 레벨 확인 변수
 */
int write_change_list(file_node *head, int idx, int status, bool is_first) // 변경사항 목록 작성
{
	file_node *now;

	now = head;

	while (now != NULL) {

		switch (now->status) {
			case UNCHCK: 
				if (status == CREATE) { // 생성됨
					strcpy(change_list[idx].name, now->name);
					change_list[idx].status = CREATE;
				} else if (status == DELETE) { // 삭제됨
					char tmp[MAX_BUFFER_SIZE];
					sprintf(tmp, "%.*s/%s", (int)strlen(dst_path), dst_path, get_file_name(src_path));
					if(strstr(now->name, tmp) == NULL || !strcmp(now->name, tmp))
						break;
					strcpy(change_list[idx].name, now->name);
					change_list[idx].status = DELETE;
				}
				change_list[idx++].size = now->size;
#ifdef DEBUG
				printf("write_change_list(): change_list[%d] = %s(%dbyte), status = %d\n", idx - 1, change_list[idx - 1].name, change_list[idx - 1].size, change_list[idx - 1].status);
#endif
				break;

			case MODIFY: // 수정됨
				strcpy(change_list[idx].name, now->name);
				change_list[idx].status = MODIFY;
				change_list[idx++].size = now->size;
#ifdef DEBUG
				printf("write_change_list(): change_list[%d] = %s(%dbyte), status = %d\n", idx - 1, change_list[idx - 1].name, change_list[idx - 1].size, change_list[idx - 1].status);
#endif
				break;
		}

		if(option_r || is_first) // R옵션이 존재하거나 첫번째 레벨일 경우
			if (now->child != NULL)
				idx = write_change_list(now->child, idx, status, false);

		now = now->next;
	}

	return idx;
}

/**
 * @brief 파일 동기화
 * @param count 변경 사항 개수
 */
void renewal(int count) // 파일 동기화
{
	int src_fd, dst_fd;
	char path[MAX_BUFFER_SIZE];
	char buf[MAX_BUFFER_SIZE];
	struct stat statbuf;
	struct utimbuf attr;
	size_t length;

	// 타겟 디렉토리가 존재하지 않을경우 동기화 디렉토리에 생성
	sprintf(path, "%.*s/%s", (int)strlen(dst_path), dst_path, get_file_name(src_path)); 
	if (src_is_dir && access(path, F_OK) < 0) {
		lstat(src_path, &statbuf);
		mkdir(path, statbuf.st_mode);
	}

	for (int i = 0; i < count; i++) {

		switch (change_list[i].status) {
			case DELETE:

				lstat(change_list[i].name, &statbuf);
				if (S_ISDIR(statbuf.st_mode))
					remove_directory(change_list[i].name);
				else
					remove(change_list[i].name);
				break;

			case CREATE:
			case MODIFY:

				memset(path, 0, MAX_BUFFER_SIZE);

				lstat(change_list[i].name, &statbuf);
				sprintf(path, "%.*s/%s", (int)strlen(dst_path), dst_path, change_list[i].name + strlen(src_path) - strlen(get_file_name(src_path))); // 동기화 파일 경로 생성
#ifdef DEBUG
				printf("renewal: path = %s\n", path);
#endif

				if (S_ISDIR(statbuf.st_mode)) 
					mkdir(path, 0755);
				else {
					if ((src_fd = open(change_list[i].name, O_RDONLY)) < 0) { // 타겟 읽기 전용 열기
						fprintf(stderr, "renewal(): open error for %s\n", change_list[i].name);
						break;
					}

					if ((dst_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, statbuf.st_mode)) < 0) { // 동기화 파일 열기
						fprintf(stderr, "renewal(): open error for %s\n", path);
						break;
					}

					while ((length = read(src_fd, buf, MAX_BUFFER_SIZE)) > 0) // 동기화 파일 쓰기 (새로쓰기 혹은 덮어쓰기)
						write(dst_fd, buf, length);

					close(src_fd);
					close(dst_fd);
				}

				// 동기화 파일 속성 및 권한 복원
				attr.actime = statbuf.st_atime; 
				attr.modtime = statbuf.st_mtime;
				utime(path, &attr);
				chmod(path, statbuf.st_mode);
				break;
		}
	}
	write_log(count, 0);
}

/**
 * @brief tar 동기화
 * @param count 변경 사항 개수
 */
void renewal_tar(int count) 
{
	char file_name[BUFFER_SIZE] = { 0 };
	char path[MAX_BUFFER_SIZE] = { 0 }; 
	char command[MAX_BUFFER_SIZE] = { 0 };
	struct stat statbuf;
	unsigned long long size;

	if (count == 0) {
		fprintf(stderr, "write_log(): already up to date in %s\n", dst_path);
		return;
	}

	// 1. 타겟 디렉토리 tar 생성
	strncpy(path, src_path, get_file_name(src_path) - src_path - 1); 
	sprintf(file_name, "%s.tar", get_file_name(src_path));
#ifdef DEBUG
	printf("renewal_tar(): cd %s\n", path);
#endif
	chdir(path);

	// 2. 압축 실행
	for(int i = 0; i < count; i++) {
		switch (change_list[i].status) {
			case DELETE:

				lstat(change_list[i].name, &statbuf);
				if (S_ISDIR(statbuf.st_mode))
					remove_directory(change_list[i].name);
				else
					remove(change_list[i].name);
				break;

			case CREATE:
			case MODIFY:

				// 압축 파일에 파일 추가
				if (src_is_dir)
#ifdef DEBUG
					sprintf(command, "tar -rvf %s %s", file_name, change_list[i].name + strlen(src_path) - strlen(get_file_name(src_path))); 
#else
					sprintf(command, "tar -rf %s %s", file_name, change_list[i].name + strlen(src_path) - strlen(get_file_name(src_path)));
#endif				
				else 
#ifdef DEBUG
					sprintf(command, "tar -rvf %s %s", file_name, get_file_name(src_path));
#else
					sprintf(command, "tar -rf %s %s", file_name, get_file_name(src_path));
#endif
#ifdef DEBUG
				printf("renewal_tar(): command = %s\n", command);
#endif
				system(command); // 압축 실행
				break;
		}
	}

	chdir(path);
	lstat(file_name, &statbuf); // 상태 정보 획득
	size = statbuf.st_size; // 압축 파일 크기 획득
	sprintf(path, "%.*s/%.*s", (int)strlen(dst_path), dst_path, (int)strlen(file_name), file_name); // 압축 풀 경로 생성
	rename(file_name, path); // 압축 파일 이동
#ifdef DEBUG
	printf("renewal_tar(): cd %s\n", dst_path);
#endif
	chdir(dst_path);
#ifdef DEBUG
	sprintf(command, "tar -xvf %s", file_name);
	printf("renewal_tar(): command = %s\n", command);
#else
	sprintf(command, "tar -xf %s", file_name);
#endif
	system(command);
	unlink(file_name);
	chdir(pwd);

	write_log(count, size);
}

/**
 * @brief 로그 파일 작성
 * @param count 변경 사항 개수
 */
void write_log(int count, unsigned long long totalsize) // 로그 파일 작성
{
	FILE *fp;
	time_t now_t;
	struct tm *now_tm;
	char command[MAX_BUFFER_SIZE];

	if (count == 0) {
		fprintf(stderr, "write_log(): already up to date in %s\n", dst_path);
		return;
	}

	if ((fp = fopen(RSYNC_LOG, "r+")) == NULL) // 로그파일 열기
		fp = fopen(RSYNC_LOG, "w"); // 존재하지 않을 경우 생성

	fseek(fp, 0, SEEK_END); 

	// 헤더 명령행 문자열 생성
	strcpy(command, get_file_name(saved_argv[0]));
	for (int i = 1; i < saved_argc; i++) {
		strcat(command, " ");
		strcat(command, saved_argv[i]);
	}

#ifdef DEBUG
	printf("write_log(): command = %s\n", command);
#endif

	// 헤더 시간 생성
	time(&now_t);
	now_tm = localtime(&now_t);

	fprintf(fp, "[%.24s] %s\n", asctime(now_tm), command); // 헤더 라인 쓰기

	if (option_t)
		fprintf(fp, "        totalsize %llubytes\n", totalsize);

	for (int i = 0; i < count; i++) // 변경 사항 쓰기
		switch (change_list[i].status) {
			case DELETE:
				fprintf(fp, "        %s delete\n",  change_list[i].name + strlen(dst_path) + 1);
				break;
			case CREATE:
			case MODIFY:
				if (src_is_dir) 
					if (option_t)
						fprintf(fp, "        %s\n", change_list[i].name + strlen(src_path) + 1);
					else
						fprintf(fp, "        %s %dbytes\n", change_list[i].name + strlen(src_path) + 1, change_list[i].size);
				else
					if (option_t)
						fprintf(fp, "        %s\n", change_list[i].name + strlen(src_path) - strlen(get_file_name(src_path)));
					else
						fprintf(fp, "        %s %dbytes\n", change_list[i].name + strlen(src_path) - strlen(get_file_name(src_path)), change_list[i].size);
				break;
		}
}

/**
 * @brief 모니터링 파일 목록 메모리 할당 해제
 * @param head 트리의 루트 노드
 */
void free_list(file_node *head) // 모니터링 파일 목록 메모리 할당 해제
{
	// 모든 노드들을 찾아서 메모리 할당을 해제한다.
	if(head->child != NULL) // 자식 탐색
		free_list(head->child);

	if(head->next != NULL) // 형제 탐색
		free_list(head->next);

	free(head->namelist);
	free(head); // 메모리 엑세스 허용
}

/**
 * @brief SIGINT 시그널 처리
 * @param signo 시그널
 */
void recovery(int signo) // SIGINT 시그널 처리
{
	char command[MAX_BUFFER_SIZE];
	char path[MAX_BUFFER_SIZE];

	if(signo == SIGINT) { // SIGINT 시그널 획득 시
#ifdef DEBUG
		printf("recovery(): SIGINT signal is arrived\n");
#endif
		if(is_complete) // 동기화가 완료되었을 경우
			return;

		strncpy(path, dst_path, get_file_name(dst_path) - dst_path);
#ifdef DEBUG
		printf("recovery(): cd %s\n", path);
#endif
		chdir(path);

#ifdef DEBUG
		sprintf(command, "tar -xvf %s.swp", get_file_name(dst_path)); // 복원 명령어 생성(압축 해제)
		printf("recovery(): command = %s\n", command);
#else
		sprintf(command, "tar -xf %s.swp", get_file_name(dst_path)); // 복원 명령어 생성(압축 해제)
#endif
		remove_directory(dst_path); // 기존 동기화 디렉토리 삭제
		system(command); // 복원 명령어 실행
		unlink(command + 9); // swap 파일 삭제

	}
	exit(1);
}

/**
 * @brief 디렉토리 삭제
 * @param path 삭제할 디렉토리 경로
 */
void remove_directory(const char *path) // 디렉토리 삭제
{
	struct dirent *dirp;
	struct stat statbuf;
	DIR *dp;
	char tmp[MAX_BUFFER_SIZE];

	if ((dp = opendir(path)) == NULL)
		return;

	while ((dirp = readdir(dp)) != NULL) { // path에 존재하는 디렉토리 안에 파일들 전부 삭제
		if (!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name); // tmp = 디렉토리 내부 파일

		if (lstat(tmp, &statbuf) == -1) // 파일 상태 정보 추출
			continue;

		if (S_ISDIR(statbuf.st_mode)) // 디렉토리일 경우 재귀적으로 제거
			remove_directory(tmp);
		else 
			unlink(tmp);
	}
	closedir(dp);
	rmdir(path);
}

/**
 * @brief 파일명 반환
 * @param path 경로
 * @return 파일명 시작 메모리 주소
 */
char *get_file_name(char *path) // 파일명 추출
{
	char *tmp = path;
	int length = strlen(path);

	for (int i = 0; i < length; i++)
		if(path[i] == '/')
			tmp = path + i;

	return tmp + 1;
}

/**
 * @brief 사용법 출력
 * @param 실행파일 문자열
 */
void print_usage(char *execute_file) // 사용법 출력
{
	fprintf(stderr, "ssu_rsync(): Usage: %s [OPTION] <SOURCE> <DESTINATION>\n", execute_file);
}
