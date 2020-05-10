#include "common.h"

typedef struct ssu_commandToken { // 프롬프트 명령어 구조체
	char **argv; // 명령행 토큰
	int argc; // 명령행 인자 개수
} commands;

typedef struct ssu_fileinfo {
	int num;
	char path[BUFFER_SIZE];
	struct tm d_tm;
	struct tm m_tm;
} file_infos;

commands make_command_token(char *command_line); // 명령어 전체 문장 토큰화
int get_command_type(char *command); // COMMAND 타입 확인 및 반환
char *get_file_name(char *path); // 파일명 추출
void move_trash(file_node *head, int option_i); // 파일 휴지통 이동
void wait_thread(char *path, int sec, int option_r, int option_i); // 삭제 대기 스레드
struct tm get_tm(char *date, char *time); // 시간 구조체 획득
void remove_directory(const char *path); // 디렉토리 삭제
int check_trash_info(void); // 휴지통 파일 정보 디렉토리 크기 확인
void delete_trash_oldest(void); // 휴지통에서 가장 오래 삭제된 파일 제거
int find_trash_file(const char *file_name); // 휴지통 중복 파일 탐색
void print_list_size(file_node *head, char *path, int number, int option_d, int op_switch); // 지정 파일 상대 경로 및 크기 출력
void restore_file(const char *file_name, int option_l); // 휴지통 파일 복원
char *get_file_path(char *path, const char *file_name); // 파일 경로 추출
void sort_trash_info(const char *file_name, int idx, int delete_idx); // 삭제 후 중복 파일 번호 재정렬 
void sort_info_oldest(file_infos *file_info, int idx); // 삭제 시간 오름차순 정렬
void sort_info_order(file_infos *file_info, int idx); // 중복 파일 오름차순 정렬
void print_list_tree(file_node *head, int level, int level_check[], int is_root); // 모니터링 파일 목록 트리 출력
void print_indent(int level, int level_check[]); // 트리 출력 보조 함수
char *rtrim(char *_str); // 문자열 오른쪽 공백 제거
char *ltrim(char *_str); // 문자열 왼쪽 공백 제거
void to_lower_case(char *str); // 문자열 소문자 변환
void free_command(commands command); // 명령행 구조체 초기화
void print_usage(void); // 사용법 출력

