#include "common.h"

typedef struct ssu_commandToken { // 프롬프트 명령어 구조체
	char **argv; // 명령행 토큰
	int argc; // 명령행 인자 개수
} commands;

commands make_command_token(char *command_line); // 명령어 전체 문장 토큰화
int get_command_type(char *command); // COMMAND 타입 확인 및 반환
char *get_file_name(char *path); // 파일명 추출
int move_trash(file_node *head); // 파일 휴지통 이동
struct tm get_tm(char *date, char *time); // 시간 구조체 획득
void remove_directory(const char *path); // 디렉토리 삭제
void print_list_size(file_node *head, char *path, int number, int op_switch); // 지정 파일 상대 경로 및 크기 출력
void print_list_tree(file_node *head, int level, int level_check[], int is_root); // 모니터링 파일 목록 트리 출력
void print_indent(int level, int level_check[]); // 트리 출력 보조 함수
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void to_lower_case(char *str); // 문자열 소문자 변환
void init_option(void); // 옵션 확인 초기화
void print_usage(void); // 사용법 출력

