#include "ssu_mntr.h"

char check_path[BUFFER_SIZE] = { 0 }; // $(PWD)/check 절대경로

void mntr_process(char *pwd) // 모니터링 메인 함수
{
	FILE *fp; // log.txt 파일 구조체
	file_node *old_list = malloc(sizeof(file_node)); // 마지막 파일 목록 및 상태
	file_node *new_list = malloc(sizeof(file_node)); // 현재 파일 목록 및 상태

	if(access(CHECK, F_OK) < 0) // 모니터링 디렉토리 확인
			mkdir(CHECK, 0755); // 존재하지 않을 경우 생성

	if((fp = fopen(LOG, "w+")) == NULL) { // 로그 파일 열기 및 생성, 기존 로그 덮어쓰기
			fprintf(stderr, "fopen error for log.txt\n");
			exit(1);
	}
	fclose(fp);

	sprintf(check_path, "%s/%s", pwd, CHECK);

	while(true) {  
		new_list = make_tree(check_path); // 현재 파일 목록 및 상태 저장
	}
}

file_node *make_tree(char *path) // 디렉토리 파일 목록 트리화
{
	// 파일    : 노드 생성(절대경로/이름, 상태정보)
	// 디렉토리: 트리 생성

	int file_count;
	int isRoot = true;
	int i;
	file_node *head = malloc(sizeof(file_node));
	file_node *now = malloc(sizeof(file_node));

	now = head;

	strcpy(head->name, path); // 현재 경로 저장
	stat(head->name, &(head->status)); // 상태 정보 저장 

	file_count = scandir(head->name, &(head->namelist), NULL, alphasort); // 현재 경로의 모든 파일 탐색 및 개수 저장

	for(i = 0; i < file_count; i++) {

		if(!strcmp(head->namelist[i]->d_name, ".") || !strcmp(head->namelist[i]->d_name, "..")) // 현재, 상위 디렉토리 접근 지정자 생략
			continue;

		file_node *new = malloc(sizeof(file_node));
		new->child = NULL; // 하위 목록 초기화
		new->next = NULL; // 동일 경로 연결 파일 초기화

		sprintf(new->name, "%s/%s", path, head->namelist[i]->d_name);
		stat(new->name, &(new->status));

		if(S_ISDIR(new->status.st_mode)) // 현재 경로의 파일 목록 중 탐색한 파일이 디렉토리일 경우
			new = make_tree(new->name); // 해당 디렉토리 파일 목록 트리화

		if(isRoot) { // 현재 노드가 루트일 경우 자식으로 연결
			now->child = new;
			now = now->child;
			isRoot = false;
		} else { // 아닐 경우 형제로 연결
			now->next = new;
			now = now->next;
		}
	}
	return head;
}
