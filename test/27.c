#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE MAXNAMLEN // 디렉토리 이름 길이 매크로
#define isdigit(x) (x >= '0' && x <= '9') ? 1 : 0 // 숫자 판단 매크로
#define MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH // 권한 매크로, 664

int create_dir(int depth, char *cur_dir); // 디렉토리 생성 함수
void writefile(char *in_f, char *out_f); // 파일을 복사하는 함수
void change_mod(const char *file_path); // 모드를 변경하는 함수

char *fname = "ssu_test.txt"; // 생성하고 복사할 파일의 기본 이름
int o_flag = 0, e_flag = 0; // e 옵션과 o 옵션을 나타낼 플래그

int main(int argc, char *argv[])
{
	int opt; // 옵션 인자를 받을 변수
	int depth = 0; // 하위 디렉토리의 개수를 받을 변수
	char cur_dir_name[DIRECTORY_SIZE] = { "\0", }; // 현재 디렉토리 이름
	int i;
	int fd;

	while ((opt = getopt(argc, argv, "e:o:")) != -1) {
		switch (opt) {
			case 'e':
				e_flag = 1;
				for(i = 0; i < strlen(optarg); i++)
					if(!isdigit(optarg[i])) {
						fprintf(stderr, "invalid number\n");
						exit(1);
					}
				depth = atoi(optarg);
				break;
			case 'o':
				o_flag = 1;
				for(i = 0; i < strlen(optarg); i++)
					if(!isdigit(optarg[i])) {
						fprintf(stderr, "invalid number\n");
						exit(1);
					}
				depth = atoi(optarg);
				break;
			case '?':
				break;
		}
	}

	if (argc < 3) {
		

	} else {
		fprintf(stderr, "too many argv\n");
		exit(1);
	}

	if ((fd = creat(fname, MODE)) < 0) {
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	} else 
		close(fd);

	getcwd(cur_dir_name, MAXNAMLEN);
	sprintf(cur_dir_name, "%s/0/", cur_dir_name);
	if (mkdir(cur_dir_name, 0777)) {
		fprintf(stderr, "mkdir error\n");
		exit(1);
	}

	create_dir(depth, cur_dir_name);
	exit(0);
}

int create_dir(int depth, char *cur_dir)
{
	int i = 0;
	char tmp_filename[MAXNAMLEN] = { '\0', };
	struct stat dir_st;

	while (cur_dir[i] != '\0') 
		i++;

	if (stat(cur_dir, &dir_st) < 0) {
		fprintf(stderr, "stat error for %s\n", cur_dir);
		return 0;
	}

	strcat(tmp_filename, cur_dir);
	sprintf(tmp_filename, "%s%d", tmp_filename, depth);
	mkdir(tmp_filename, 0777);
	sprintf(tmp_filename, "%s%s", tmp_filename, fname);

	if(o_flag) {
		if (depth % 2 == 1)
			writefile(fname, tmp_filename);
	} else if (e_flag) {
		if (depth % 2 == 0)
			writefile(fname, tmp_filename);
	} else if (!o_flag && !e_flag) {

	}

	if (depth == 0)
		return 0;
	sprintf(cur_dir, "%s%d/", cur_dir, depth);

	return create_dir(depth - 1, cur_dir);
}

void writefile(char *in_f, char *out_f)
{
	FILE *in_fp, *out_fp;
	size_t length;
	char buf[1024];

	if ((in_fp = fopen(in_f, "r")) == NULL) {
		fprintf(stderr, "fopen error for %s\n", in_f);
		exit(1);
	}

	if ((out_fp = fopen(out_f, "w+")) == NULL) {
		fprintf(stderr, "fopen1 error for %s\n", out_f);
		exit(1);
	}

	while ((length = fread(buf, 1024, 1, in_fp)) > 0)
		fwrite(buf, 1024, 1, out_fp);

	fclose(in_fp);
	fclose(out_fp);
	change_mod(out_f);
}

void change_mod(const char *file_path)
{
	chmod(file_path, MODE);
}


