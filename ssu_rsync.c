/**
 * @file ssu_rsync.c
 * @brief ssu_rsync 프로그램
 * @author 김병준 (kbj9704@gmail.com)
 */
#include "ssu_rsync.h"

/**
 * @brief ssu_rsync 메인 함수
 * @param argc 인자 개수
 * @param argv 인자 문자열
 */
int main(int argc, char *argv[])
{
	// 실행 시간 측정 
	struct timeval begin_t, end_t;

	// 파일 경로
	char src_path[MAX_BUFFER_SIZE];
	char dst_path[MAX_BUFFER_SIZE];

	// 옵션
	bool option_r = false;
	bool option_t = false;
	bool option_m = false;

	// 유효 검사
	bool is_invalid = false;
	bool is_src = false;
	bool is_dst = false;


	gettimeofday(&begin_t, NULL); // 측정 시작

	if (argc < 3) { // 인자 개수가 부족할 경우
		fprintf(stderr, "ssu_rsync(): Usage: %s [OPTION] <SOURCE> <DESTINATION>\n", argv[0]);
		exit(1);
	}

	for (int i = 0; i < argc; i++) {

		// 옵션 파싱
		if (argv[i][0] == '-') {
			if (!strcmp(argv[i], "-r"))
				option_r = true;
			else if (!strcmp(argv[i], "-t"))
				option_t = true;
			else if (!strcmp(argv[i], "-m"))
				option_m = true;
			else {
				is_invalid = true;
				break;
			}
			continue;
		}

		// 목적 경로 파싱
		if (!is_src) {
			if (access(argv[i], F_OK) < 0) {
				fprintf(stderr, "ssu_rsync(): access error for %s\n", argv[i]);
				is_invalid = true;
				exit(1);
			} else 
				realpath(argv[i], src_path); // 절대 경로로 변환

			is_src = true;
			continue;
		}

		// 위치 경로 파싱
		if (!is_dst) {
			if (access(argv[i], F_OK) < 0) {
				fprintf(stderr, "ssu_rsync(): access error for %s\n", argv[i]);
				is_invalid = true;
				exit(1);
			} else 
				realpath(argv[i], dst_path); // 절대 경로로 변환

			is_dst = true;
			continue;
		}
	}

	// 파싱 중 에러 발견
	if (is_invalid)
		exit(1);
	else if (!is_src) {
		fprintf(stderr, "ssu_rsync(): <SOURCE> doesn't exist\n");
		exit(1);
	} else if (!is_dst) {
		fprintf(stderr, "ssu_rsync(): <DESTINATION> doesn't exist\n");
		exit(1);
	}
	
	gettimeofday(&end_t, NULL); // 측정 종료
	ssu_runtime(&begin_t, &end_t); // 실행 시간 출력
	exit(0);
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
 * @return 트리의 루트노드
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

		sprintf(new->name, "%s/%s", path, head->namelist[i]->d_name); // 파일 이름 저장
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
