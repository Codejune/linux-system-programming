#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/time.h>

// 불 타입
#ifndef true
    #define true  1
#endif
#ifndef false
    #define false 0
#endif

// 표준 입출력 타입
#ifndef STDOUT 
    #define STDOUT 1
#endif
#ifndef STDERR
    #define STDERR 2
#endif

// 명령어 타입
#ifndef DELETE
    #define DELETE  1
#endif
#ifndef SIZE
    #define SIZE    2
#endif
#ifndef RECOVER
    #define RECOVER 3
#endif
#ifndef TREE
    #define TREE    4
#endif
#ifndef EXIT
    #define EXIT    5
#endif
#ifndef HELP
    #define HELP    6
#endif
#ifndef UNKNOWN
    #define UNKNOWN 7
#endif

// 크기
#define DATE_SIZE       10
#define TIME_SIZE       8
#define BUFFER_SIZE     256
#define MAX_BUFFER_SIZE 1024
#define MAX_INFO_SIZE   2048

// 디렉토리
#define CHECK             "check"
#define TRASH             "trash"
#define TRASH_FILES TRASH "/files"
#define TRASH_INFO  TRASH "/info"
#define LOG               "log.txt"

// 모니터링 상태
#define UNCHCK -1
#define CHCKED  0
#define CREATE  2
#define MODIFY  3

// 형식
#define YYMMDD_HHMMSS   "%.4d-%.2d-%.2d %.2d:%.2d:%.2d"
#define YYMMDD          "%.4d-%.2d-%.2d"
#define HHSS            "%.2d:%.2d"
#define PROMPT          "20162448>"
#define SECOND_TO_MICRO 1000000

// 권한
#define DIR_MODE 0755

typedef struct ssu_fileNode{ // 모니터링 파일 목록 구조체
	char name[BUFFER_SIZE]; // 파일 이름
	struct stat attr; // 파일 상태 정보
	struct dirent **namelist; // 디렉토리 경우 하위 파일 목록
	struct ssu_fileNode *next; // 하위 디렉토리 파일 포인터
	struct ssu_fileNode *child; // 같은 레벨의 다음 파일 포인터
	int size; // 파일 크기
	int status; // 모니터링 확인 상태
} file_node;

void prompt(void); // 프롬프트
void monitoring(void); // 모니터링 
file_node* make_list(char *path); // 디렉토리 파일 목록 트리화
file_node *make_node(void); // 노드 생성
int count_size(file_node *head); // 디렉토리 크기 반환
void free_list(file_node *head); // 파일 목록 구조체 메모리 할당 해제 
char *make_time_format(struct tm time); // 시간 형식 문자열 생성 
void ssu_runtime(struct timeval *begin_t, struct timeval *end_t); // 실행시간 출력
