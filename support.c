#include "common.h"

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
	head->size = count_size(head->child);
	return head;
}

int count_size(file_node *head) // 디렉토리 크기 반환
{
	file_node *now;
	int size;

	size = false;
	now = head;

	while(now != NULL) {
		size += now->size;
		now = now->next;
	}

	return size;
}

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

char *make_time_format(struct tm time) // 시간 형식 문자열 생성
{
	static char time_format[BUFFER_SIZE];

	sprintf(time_format, YYMMDD_HHMMSS,
			time.tm_year + 1900,
			time.tm_mon + 1,
			time.tm_mday,
			time.tm_hour,
			time.tm_min,
			time.tm_sec);

	return (char*)time_format;
}

