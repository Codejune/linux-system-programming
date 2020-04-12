#ifndef _2019_LINUX_SYSTEM_PROGRAMMING_FIRST_PROJECT_
#define _2019_LINUX_SYSTEM_PROGRAMMING_FIRST_PROJECT_
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0
#define SECOND_TO_MICRO 1000000
#define STRLEN 256
#define EXTENDED_STRLEN (256*4) // 1KB
#define OPTLEN 5

#define WRONG 0
#define WARNING 0.1
#define TIMEOUT 5

#define OPERATOR_CNT 24
#define DATATYPE_SIZE 35
#define MINLEN 64
#define TOKEN_CNT 50

typedef _Bool bool;

typedef struct _option
{
	bool opt_h;
	bool opt_e; int idx_e;
	bool opt_t; int idx_t;
	bool opt_c; int idx_c;
	bool opt_p; int idx_p;
}Options;

typedef struct _problem
{
	char pid[STRLEN]; // 문제 번호
	double score; // 점수
}Problem;

typedef struct _student
{
	double sum;
	char sid[STRLEN]; // 학번
	Problem *prob; // 문제
}Student;

/* 빈칸 채점 */

typedef struct node
{
	int parentheses; // 괄호
	char *name; 
	struct node *parent; // 부모
	struct node *child_head; // 자식 head
	struct node *prev; // 이전
	struct node *next; // 다음
}node;

typedef struct operator_precedence
{
	char *operator; // 연산자
	int precedence; // 우선순위
}operator_precedence;

bool isAppend; // 학생 구조체까지 만들어진다면
int num_std; // 학생 수
int num_ans; // 문제 수
int saved_argc; // argc값 저장
char **saved_argv; // argv값 저장
Options options; // option 확인
Student *students; // 학생 관리 구조체

char fpwd[STRLEN]; // present working directory file
char fcsv[STRLEN]; // csv file
char fpnt[STRLEN]; // point file
char fstd[STRLEN]; char fans[STRLEN]; // 학생파일, 답안파일 이름(절대경로)
char ferr[STRLEN]; // error directory path(절대경로)
struct dirent ** flist_std; int count_std; // student files
struct dirent ** flist_ans; int count_ans; // answer files
struct timeval begin,end; // time checking value

/* support.c */

void ssu_runtime(struct timeval * begin_t,struct timeval * end_t); // 실행 시간 측정

void input_check(int argc,char *argv[]); // main의 입력값 검사

void before_grading(void); // 채점 준비

void grading(void); // 채점

void answer_compile(void);

void save_stdout(const char * exe_out,const char * execute,bool isStdout);

bool isModified(const char * f,struct stat statbuf); // 수정되었는가?

void make_dir(const char * path); // 디렉토리 생성 함수

void file_copy(const char * from,const char * to); // file copy

void file_remove(const char * file,bool isDir);

void make_score_table(char choice); // 채점 기준 점수표 만들기

void out_of_arguments(bool option,int idx); // 옵션의 인자 수 초과 처리

void option_help(void); // option -h

void option_c(bool check);

void make_result(void); // 결과 테이블 구성

double option_p(int std_idx); // option -p

void free_allocated_mem(void); // 동적할당 메모리 해제

void error_message(char * message); // error message

/* blank.c */

int blank_main(int std_idx,int ans_idx);

bool isMatched(const char * fstd_txt,const char * fans_txt); // 빈칸 비교

void remove_space(char * buf); // 공백문자 제거

char * get_answer(int fd,char *result);

void compare_tree(node *root1,  node *root2, int *result); // 트리 비교 함수

node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses); // 트리 구성

node *change_sibling(node *parent); // 자식 교체

node *create_node(char *name, int parentheses); // 노드 생성 함수

int get_precedence(char *op); 

int is_operator(char *op); // 연산자인가?

void print(node *cur); // 트리 출력

node *get_operator(node *cur);

node *get_root(node *cur);

node *get_high_precedence_node(node *cur, node *new);

node *get_most_high_precedence_node(node *cur, node *new);

node *insert_node(node *old, node *new);

node *get_last_child(node *cur);

void free_node(node *cur);

int get_sibling_cnt(node *cur);

int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN]);

int is_typeStatement(char *str);

int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN]);

int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN]);

int is_character(char c);

int all_star(char *str);

int all_character(char *str);

int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN]);

void clear_tokens(char tokens[TOKEN_CNT][MINLEN]);

int get_token_cnt(char tokens[TOKEN_CNT][MINLEN]);

char *rtrim(char *_str);

char *ltrim(char *_str);

void remove_space(char *str);

int check_brackets(char *str);

char* remove_extraspace(char *str);

/* program.c */

int program_main(int std_idx,int ans_idx);

int find_prob_idx(const char * prob_name,int std_idx); // index return

void student_compile(int std_idx,int ans_idx); // compile

int option_limit_idx(int idx_opt); // option의 매개변수의 끝 부분 index return

bool find_error(const char * file,struct stat statbuf,int std_idx,int prob_idx); // error or warning or not?

void student_execute(int std_idx,int ans_idx); // 실행결과 판단

void student_stdout(int std_idx,int ans_idx); // 실행결과 비교

void * execute_process(void * arg); // background execute

bool isTimeout(char * process); // 시간초과 제어

#endif
