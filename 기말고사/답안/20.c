#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define SHORT_BUF 1024
#define MAX_BUF 4096
#define ASCII_MAX 256
#define ARG_MAX 16

char *arg_container[ASCII_MAX][ARG_MAX];
int arg_ptr[ASCII_MAX];

int ssu_proc_pid_fd(void);
int ssu_proc_pid_fd_derived(char*);

int main(void)
{
	ssu_proc_pid_fd();
	exit(0);
}

int ssu_proc_pid_fd(void)
{
	char fd_path[MAX_BUF] = { 0 }; // /proc/[PID]/fd의 경로를 저장하는 배열
	char pid_buf[SHORT_BUF] = { 0 }; // PID를 문자열으로 저장하는 배열
	char opt = 'f'; // 옵션을 나타내는 문자
	int j = 0;

}

int ssu_proc_pid_fd_derived(char *fd_path)
{
	char fd_res[SHORT_BUF] = { 0 }; // 심볼릭 링크를 readlink()로 읽은 결과를 저장하는 배열
	char fd_tmp[SHORT_BUF] = { 0 }; // 문자열 조작에 임시적으로 사용할 수 있는 배열
	DIR *dir_ptr = NULL;
	struct dirent *dir_entry = NULL;

}


