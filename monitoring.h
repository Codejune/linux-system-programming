#include "common.h"

typedef struct ssu_changeItem { // 변경사항 구조체
	time_t time;
	char name[BUFFER_SIZE];
	int status;
} change_file;

void monitoring(void); // 모니터링
void init_list_status(file_node *head, int status); // 모니터링 파일 상태 초기화 
int count_file(file_node *head); // 파일 개수 반환 
void compare_list(file_node *new_list, file_node *old_list); // 파일 목록 트리 비교
int compare_file(file_node *new_file, file_node *old_file); // 파일 정보 비교
int write_change_list(file_node *head, int idx, int status); // 변경 사항 목록 작성
void sort_change_list(int idx); // 변경사항 목록 시간순 정렬
void write_change_log(int idx); // 변경사항 파일 기록
void set_daemon_process(void); // 데몬프로세스 설정
