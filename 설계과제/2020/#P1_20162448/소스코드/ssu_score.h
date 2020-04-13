// ifndef: 각 파일에서 선언한 상수 중복 선언 방지
#ifndef MAIN_H_
#define MAIN_H_

// 참, 거짓
#ifndef true
	#define true 1
#endif
#ifndef false
	#define false 0
#endif
// 표준 입출력
#ifndef STDOUT
	#define STDOUT 1
#endif
#ifndef STDERR
	#define STDERR 2
#endif
// 파일 타입
#ifndef TEXTFILE
	#define TEXTFILE 3
#endif
#ifndef CFILE
	#define CFILE 4
#endif
// 타임아웃 
#ifndef OVER
	#define OVER 5
#endif
// 채점 점수
#ifndef WARNING
	#define WARNING -0.1
#endif
#ifndef ERROR
	#define ERROR 0
#endif
// 버퍼 크기
#define FILELEN 64
#define BUFLEN 1024
#define SNUM 100
#define QNUM 100
// 최대 허용 인자
#define ARGNUM 5
// 초->마이크로초 변환
#define SECOND_TO_MICRO 1000000

struct ssu_scoreTable{ // 점수 테이블 구조체
	char qname[FILELEN]; // 문제 번호
	double score; // 점수
};

// ssu_score.c
void ssu_score(int argc, char *argv[]);
int check_option(int argc, char *argv[]);
void print_usage();
void score_students();
double score_student(int fd, char *id);
void write_first_row(int fd);
char *get_answer(int fd, char *result);
int score_blank(char *id, char *filename);
double score_program(char *id, char *filename);
double compile_program(char *id, char *filename);
int execute_program(char *id, char *filname);
pid_t inBackground(char *name);
double check_error_warning(char *filename);
int compare_resultfile(char *file1, char *file2);
void do_iOption(char (*ids)[FILELEN]);
int get_header_idx(char *header, int comma_cnt);
char *get_header_char(char *header, int idx);
int is_exist(char (*src)[FILELEN], char *target);
int is_thread(char *qname);
void redirection(char *command, int newfd, int oldfd);
int get_file_type(char *filename);
void rmdirs(const char *path);
void to_lower_case(char *c);
void set_scoreTable(char *ansDir);
void read_scoreTable(char *path);
void make_scoreTable(char *ansDir);
void write_scoreTable(char *filename);
void set_idTable(char *stuDir);
int get_create_type();
void sort_idTable(int size);
void sort_scoreTable(int size);
void get_qname_number(char *qname, int *num1, int *num2);
void do_mOption();
int rescore(int line, double new_score);
void file_remove(const char *file, _Bool isDir);
void file_copy(const char *from, const char *to);
#endif
