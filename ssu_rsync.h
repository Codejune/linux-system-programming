/**
 * @file ssu_rsync.h
 * @brief ssu_rsync에서 사용되는 라이브러리, 매크로, 프로토타입 선언
 * @author 김병준 (kbj9704@gmail.com)
 */
#ifndef SSU_RSYNC_H
#define SSU_RSYNC_H

#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

/**
 * @brief diff 상태
 */
#define UNCHCK -1
#define CHCKED  0
#define DELETE  1
#define CREATE  2
#define MODIFY  3

/**
 * @brief 파일 이름
 */
#define RSYNC_LOG "ssu_rsync_log"

typedef struct ssu_fileNode{ // 파일 목록 구조체
	char name[BUFFER_SIZE]; // 파일 이름
	struct stat attr; // 파일 상태 정보
	struct dirent **namelist; // 디렉토리 경우 하위 파일 목록
	struct ssu_fileNode *next; // 하위 디렉토리 파일 포인터
	struct ssu_fileNode *child; // 같은 레벨의 다음 파일 포인터
	int size; // 파일 크기
	int status; // 확인 상태
} file_node;

void copy_argument(int argc, char *argv[]); // 명령행 인자 백업
void syncronize(char *src_path, char *dst_path); // 동기화 함수
file_node *make_node(void); // 노드 생성
file_node *make_list(char *path); // 디렉토리 파일 목록 트리화
int count_size(file_node *head); // 디렉토리 크기 반환
void compare_list(file_node *src_list, file_node *dst_list); // 파일 목록 트리 비교
bool compare_file(file_node *src_file, file_node *dst_file); // 파일 정보 비교
int write_change_list(file_node *head, int idx, int status, bool is_first); // 변경사항 목록 작성
void write_log(int count, unsigned long long totalsize); // 로그 파일 작성
void free_list(file_node *head); // 파일 목록 메모리 할당 해제
void renewal(int count); // 파일 동기화
void renewal_tar(int count); // tar 동기화
void recovery(int signo); // SIGINT 시그널 처리
void remove_directory(const char *path); // 디렉토리 삭제
char *get_file_name(char *path); // 파일명 추출
void print_usage(char *execute_file); // 사용법 출력
#endif // SSU_RSYNC_H
