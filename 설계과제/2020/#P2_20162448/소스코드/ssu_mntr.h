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
struct ssu_fileTable{ // 모니터링 리스트 테이블
	struct stat status;
	char name[BUFFER_SIZE];
};


// ssu_mntr.c
void ssu_mntr(void); // 프롬프트 메인 함수
int get_command_type(char *command); // COMMAND 타입 확인 및 반환
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void print_usage(void);

// mntr_process.c
void mntr_process(void); // 모니터링 메인 함수
void set_fileTable(void); // 모니터링 리스트 테이블 생성
void check_change(void);








