#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

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

// 버퍼
#define BUFFER_SIZE     256
#define MAX_BUFFER_SIZE 1024

// 디렉토리
#define CHECK "check"
#define TRASH "trash"
#define LOG   "log.txt"

// 모니터링 상태
#define UNCHCK -1
#define CHCKED  0
#define CREATE  2
#define MODIFY  3

// 형식
#define YYMMDD_HHMMSS "%.4d-%.2d-%.2d %.2d:%.2d:%.2d"
#define YYMMDD        "%.4d-%.2d-%.2d"
#define HHSS          "%.2d:%.2d"
#define PROMPT        "20162448>"

// 권한
#define DIR_ACCESS_MODE 0755

typedef struct ssu_fileNode{ // 모니터링 파일 목록 구조체
	char name[BUFFER_SIZE]; // 파일 이름
	struct stat attr; // 파일 상태 정보
	struct dirent **namelist; // 디렉토리 경우 하위 파일 목록
	struct ssu_fileNode *next; 
	struct ssu_fileNode *child;
	int status;
} file_node;

typedef struct ssu_changeItem {
	time_t time;
	char name[BUFFER_SIZE];
	int status;
} change_file;

typedef struct ssu_commandToken {
	char **argv; // 명령행 토큰
	int argc; // 명령행 인자 개수
} commands;


// ssu_mntr.c
void ssu_mntr(char *pwd); // 프롬프트 메인 함수
commands make_command_token(char *command_line); // 명령어 전체 문장 토큰화
//int check_option(int argc, char *argv[]); // 옵션 인자 검사
int get_command_type(char *command); // COMMAND 타입 확인 및 반환
void print_list_tree(file_node *head, int level, int level_check[], int is_root); // 모니터링 파일 목록 트리 출력
void print_indent(int level, int level_check[]); // 트리 출력 보조 함수
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void to_lower_case(char *str); // 문자열 소문자 변환
void init_option(void); // 옵션 확인 초기화
void print_usage(void);

// mntr_process.c
void mntr_process(char *pwd); // 모니터링 메인 함수
file_node* make_list(char *path); // 디렉토리 파일 목록 트리화
int count_file(file_node *head); // 주어진 목록 파일 개수
void init_list_status(file_node *head, int status); // 모니터링 파일 상태 초기화 
void compare_list(file_node *new_list, file_node *old_list); // 파일 목록 트리 비교
int compare_file(file_node *new_file, file_node *old_file); // 파일 정보 비교
int write_change_list(file_node *head, int idx, int status); // 변경 사항 목록 작성
void sort_change_list(int idx); // 변경사항 목록 시간순 정렬
void write_change_log(int idx); // 변경사항 파일 기록
void free_list(file_node *head); // 모니터링 파일 목록 메모리 할당 해제
