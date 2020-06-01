/**
 * @file ssu_rsync.h
 * @brief ssu_rsync에서 사용되는 라이브러리, 매크로, 프로토타입 선언
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef SSU_RSYNC_H
#define SSU_RSYNC_H

#include <dirent.h>
#include <sys/stat.h>
#include "common.h"

/**
 * @brief diff 상태
 */
#define UNCHCK -1
#define CHCKED  0
#define CREATE  2
#define MODIFY  3

/**
 * @brief 파일 이름
 */
#define RSYNC_LOG "ssu_rsync_log"

typedef struct ssu_fileNode{ // 모니터링 파일 목록 구조체
	char name[BUFFER_SIZE]; // 파일 이름
	struct stat attr; // 파일 상태 정보
	struct dirent **namelist; // 디렉토리 경우 하위 파일 목록
	struct ssu_fileNode *next; // 하위 디렉토리 파일 포인터
	struct ssu_fileNode *child; // 같은 레벨의 다음 파일 포인터
	int size; // 파일 크기
	int status; // 모니터링 확인 상태
} file_node;

void syncronized(char *src_path, char *dst_path); // 동기화 함수
file_node *make_node(void); // 노드 생성
file_node *make_list(char *path); // 디렉토리 파일 목록 트리화
int count_size(file_node *head); // 디렉토리 크기 반환
void compare_list(file_node *src_list, file_node *dst_list); // 파일 목록 트리 비교
int compare_file(file_node *src_file, file_node *dst_file); // 파일 정보 비교
void free_list(file_node *head); // 모니터링 파일 목록 메모리 할당 해제
#endif // SSU_RSYNC_H
