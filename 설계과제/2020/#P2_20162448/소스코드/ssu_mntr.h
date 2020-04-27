#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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

// main.c

// ssu_mntr.c
int ssu_mntr(int argc, char *argv[]); // 프롬프트 메인 함수
int get_command_type(char *command); // COMMAND 타입 확인 및 반환
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void print_usage(void);








