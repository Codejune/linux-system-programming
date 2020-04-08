#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_score.h"
#include "blank.h"

extern struct ssu_scoreTable score_table[QNUM];
extern char id_table[SNUM][10];

struct ssu_scoreTable score_table[QNUM];
char id_table[SNUM][10];

char saved_path[BUFLEN] = { 0 };
char stuDir[BUFLEN];
char ansDir[BUFLEN];
char errorDir[BUFLEN];
char threadFiles[ARGNUM][FILELEN];
char IDS[ARGNUM][FILELEN];

int mOption = false;
int eOption = false;
int tOption = false;
int iOption = false;
int hOption = false;

void ssu_score(int argc, char *argv[])
{
	int i;

	for(i = 0; i < argc; i++){ // -h 옵션일 경우 도움말만 출력하고 프로그램 종료
		if(!strcmp(argv[i], "-h")){
			print_usage();
			return;
		}
	}

	if(argc >= 3 && strcmp(argv[1], "-c") != 0){
		strcpy(stuDir, argv[1]);
		strcpy(ansDir, argv[2]);
	}

	if(!check_option(argc, argv))
		exit(1);

	if(!eOption && !tOption && !mOption && iOption){
		do_iOption(IDS);
		return;
	}

	getcwd(saved_path, BUFLEN); // $(PWD)

	if(chdir(stuDir) < 0){ // STD_DIR 열기
		fprintf(stderr, "%s doesn't exist\n", stuDir);
		return;
	}
	getcwd(stuDir, BUFLEN); // stuDir = $(PWD)/STD_DIR

	chdir(saved_path);
	if(chdir(ansDir) < 0){ // ANS_DIR 열기
		fprintf(stderr, "%s doesn't exist\n", ansDir);
		return;
	}
	getcwd(ansDir, BUFLEN); // ansDir = $(PWD)/ANS_DIR

	chdir(saved_path); 

	set_scoreTable(ansDir); // score_table.csv 생성
	set_idTable(stuDir); // 학생 테이블 생성

	if(mOption)
		do_mOption();

	printf("grading student's test papers..\n"); 
	score_students(); // 학생 답안 채점
	if(iOption)
		do_iOption(IDS);

	return;
}

int check_option(int argc, char *argv[])
{
	int i, j;
	int c;

	while((c = getopt(argc, argv, "e:i:thm")) != -1)
	{
		switch(c){
			case 'e':
				eOption = true;
				strcpy(errorDir, optarg);

				if(access(errorDir, F_OK) < 0)
					mkdir(errorDir, 0755);
				else{
					rmdirs(errorDir);
					mkdir(errorDir, 0755);
				}
				break;
			case 'h':
				hOption = true;
				break;
			case 't':
				tOption = true;
				i = optind;
				j = 0;

				while(i < argc && argv[i][0] != '-'){

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);
					else
						strcpy(threadFiles[j], argv[i]);
					i++; 
					j++;
				}
				break;
			case 'm':
				mOption = true;
				break;
			case 'i':
				iOption = true;
				i = optind;
				j = 0;

				while(i < argc && argv[i][0] != '-'){

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);
					else
						strcpy(IDS[j], argv[i]);
					i++; 
					j++;
				}
				break;
			case '?':
				printf("Unkown option %c\n", optopt);
				return false;
		}
	}

	return true;
}

void do_mOption( ) 
{
	FILE *fp;
	char input[BUFLEN];
	char qname[BUFLEN];
	double score;
	char *q;
	int line;
	int isEOF = 0;

	chdir(ansDir);

	while(1) {

		if((fp = fopen("score_table.csv", "r")) == NULL) {
			fprintf(stderr, "file open error for score_table.csv\n");
			return;
		}
		memset(input, 0, strlen(input));
		printf("Input question's number to modify >> ");
		scanf("%s", input);
		getchar();
		line = 0;
		while((isEOF = fscanf(fp, "%[^,],%lf\n", qname, &score)) != EOF)
		{
			q = strtok(qname, ".");
			if(strcmp(q, input) == 0) {
				printf("Current score : %g\n", score);
				printf("New score : ");
				scanf("%lf", &score);
				if(rescore(line, score)) {
					fprintf(stderr, "Rewrite error for score_table.csv\n");
					return;
				}
				isEOF = 0;
				break;
			}	
			line++;	
			memset(qname, 0 , strlen(qname));
		}
		if(strcmp(input, "no") == 0)
			break;
		if(isEOF == EOF)
			printf("Unknown question's number\n");
		fseek(fp, 0, SEEK_SET);
	}
	fclose(fp);
	chdir(saved_path);
}

int rescore(int line, double new_score) {
	FILE *origin;
	FILE *new;
	int line_cnt = 0;
	char qname[BUFLEN] = { 0 };
	char tmp[BUFLEN] = { 0 };
	char table_path[BUFLEN] = { 0 };
	char temp_path[BUFLEN] = { 0 };
	double origin_score;
	
	if((origin = fopen("score_table.csv", "r")) == NULL) {
		fprintf(stderr, "file open error for score_table.csv\n");
		return 1;
	}

	if((new = fopen("temp.csv", "w+")) == NULL) {
		fprintf(stderr, "file open error for temp.csv\n");
		return 1;
	}
	
	while(fscanf(origin, "%[^,],%lf\n", qname, &origin_score) != EOF) {
		if(line_cnt == line) {
			sprintf(tmp, "%s,%.2f\n", qname, new_score);
			fwrite(tmp, strlen(tmp), 1, new);
		} else {
			sprintf(tmp, "%s,%.2f\n", qname, origin_score);
			fwrite(tmp, strlen(tmp), 1, new);
		}
		line_cnt++;
		memset(tmp, 0, strlen(tmp));
	}
	fclose(origin);
	fclose(new);
	sprintf(table_path, "%s/%s", ansDir, "score_table.csv");
	sprintf(temp_path, "%s/%s", ansDir, "temp.csv");
	file_remove(table_path, 0);
	file_copy(temp_path, table_path);
	file_remove(temp_path, 0);
	return 0;
}

void file_remove(const char *file, _Bool isDir) 
{
	char remove[BUFLEN] = { 0 };
	if(isDir)
		sprintf(remove, "rm - r %s", file);
	else
		sprintf(remove, "rm %s", file);
	system(remove);
}

void file_copy(const char *from, const char *to) 
{
	char copy[BUFLEN] = { 0 };
	sprintf(copy, "cp %s %s", from, to);
	system(copy);
}

void do_iOption(char (*ids)[FILELEN]) // score.csv 에서 학생들의 틀린 문제 출력
{
	FILE *fp;
	char tmp[BUFLEN];
	int i = 0;
	char *p, *saved;

	if((fp = fopen("score.csv", "r")) == NULL){
		fprintf(stderr, "file open error for score.csv\n");
		return;
	}

	fscanf(fp, "%s\n", tmp);
	while(fscanf(fp, "%s\n", tmp) != EOF)
	{
		p = strtok(tmp, ",");

		if(!is_exist(ids, tmp))
			continue;

		printf("%s's wrong answer :\n", tmp);

		while((p = strtok(NULL, ",")) != NULL)
			saved = p;

		printf("%s\n", saved);
	}
	fclose(fp);
}

int is_exist(char (*src)[FILELEN], char *target)
{
	int i = 0;

	while(1)
	{
		if(i >= ARGNUM)
			return false;
		else if(!strcmp(src[i], ""))
			return false;
		else if(!strcmp(src[i++], target))
			return true;
	}
	return false;
}

void set_scoreTable(char *ansDir)
{
	char filename[FILELEN];

	sprintf(filename, "%s/%s", ansDir, "score_table.csv");

	if(access(filename, F_OK) == 0)
		read_scoreTable(filename);
	else{
		make_scoreTable(ansDir);
		write_scoreTable(filename);
	}
}

void read_scoreTable(char *path)
{
	FILE *fp;
	char qname[FILELEN];
	char score[BUFLEN];
	int idx = 0;

	if((fp = fopen(path, "r")) == NULL){
		fprintf(stderr, "file open error for %s\n", path);
		return ;
	}

	while(fscanf(fp, "%[^,],%s\n", qname, score) != EOF){
		strcpy(score_table[idx].qname, qname);
		score_table[idx++].score = atof(score);
	}

	fclose(fp);
}

void make_scoreTable(char *ansDir)
{
	int type, num;
	double score, bscore, pscore;
	struct dirent *dirp;
	DIR *dp, *c_dp;
	char tmp[BUFLEN];
	int idx = 0;
	int i;

	num = get_create_type();

	if(num == 1)
	{
		printf("Input value of blank question : ");
		scanf("%lf", &bscore);
		printf("Input value of program question : ");
		scanf("%lf", &pscore);
	}

	if((dp = opendir(ansDir)) == NULL){
		fprintf(stderr, "open dir error for %s\n", ansDir);
		return;
	}	

	while((dirp = readdir(dp)) != NULL)
	{
		sprintf(tmp, "%s/%s", ansDir, dirp->d_name);
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) // 현재, 상위 디렉토리 파일 접근 생략
			continue;

		if((type = get_file_type(dirp->d_name)) < 0) 
			continue;

		strcpy(score_table[idx++].qname, dirp->d_name); // X-X.txt | X.c

	}

	closedir(dp);
	sort_scoreTable(idx);

	for(i = 0; i < idx; i++)
	{
		type = get_file_type(score_table[i].qname);

		if(num == 1)
		{
			if(type == TEXTFILE)
				score = bscore;
			else if(type == CFILE)
				score = pscore;
		}
		else if(num == 2)
		{
			printf("Input of %s: ", score_table[i].qname);
			scanf("%lf", &score);
		}

		score_table[i].score = score;
	}
}

void write_scoreTable(char *filename)
{
	int fd;
	char tmp[BUFLEN];
	int i;
	int num = sizeof(score_table) / sizeof(score_table[0]);

	if((fd = creat(filename, 0666)) < 0){
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	for(i = 0; i < num; i++)
	{
		if(score_table[i].score == 0)
			break;

		sprintf(tmp, "%s,%.2f\n", score_table[i].qname, score_table[i].score);
		write(fd, tmp, strlen(tmp));
	}

	close(fd);
}


void set_idTable(char *stuDir) // 학번 테이블 생성
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int num = 0;

	if((dp = opendir(stuDir)) == NULL){ // STD_DIR 열기
		fprintf(stderr, "opendir error for %s\n", stuDir);
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){ // 해당 디렉토리 파일들 순회
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;

		sprintf(tmp, "%s/%s", stuDir, dirp->d_name); // STD_DIR/2020XXXX
		stat(tmp, &statbuf); // STD_DIR/내의 내용물 정보 가져오기

		if(S_ISDIR(statbuf.st_mode)) // 내용물이 디렉토리 일 경우
			strcpy(id_table[num++], dirp->d_name); // 테이블에 디렉토리 이름(학번) 저장
		else
			continue;
	}

	sort_idTable(num);
}

void sort_idTable(int size) // 학번 테이블 정렬
{
	int i, j;
	char tmp[10];

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 -i; j++){
			if(strcmp(id_table[j], id_table[j+1]) > 0){ 
				strcpy(tmp, id_table[j]);
				strcpy(id_table[j], id_table[j+1]);
				strcpy(id_table[j+1], tmp);
			}
		}
	}
}

void sort_scoreTable(int size)
{
	int i, j;
	struct ssu_scoreTable tmp;
	int num1_1, num1_2;
	int num2_1, num2_2;

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 - i; j++){

			get_qname_number(score_table[j].qname, &num1_1, &num1_2);
			get_qname_number(score_table[j+1].qname, &num2_1, &num2_2);


			if((num1_1 > num2_1) || ((num1_1 == num2_1) && (num1_2 > num2_2))){

				memcpy(&tmp, &score_table[j], sizeof(score_table[0]));
				memcpy(&score_table[j], &score_table[j+1], sizeof(score_table[0]));
				memcpy(&score_table[j+1], &tmp, sizeof(score_table[0]));
			}
		}
	}
}

void get_qname_number(char *qname, int *num1, int *num2)
{
	char *p;
	char dup[FILELEN];

	strncpy(dup, qname, strlen(qname));
	*num1 = atoi(strtok(dup, "-."));
	
	p = strtok(NULL, "-.");
	if(p == NULL)
		*num2 = 0;
	else
		*num2 = atoi(p);
}

int get_create_type()
{
	int num;

	while(1)
	{
		printf("score_table.csv file doesn't exist in TREUDIR!\n");
		printf("1. input blank question and program question's score. ex) 0.5 1\n");
		printf("2. input all question's score. ex) Input value of 1-1: 0.1\n");
		printf("select type >> ");
		scanf("%d", &num);

		if(num != 1 && num != 2)
			printf("not correct number!\n");
		else
			break;
	}

	return num;
}

void score_students() // score.csv 생성
{
	double score = 0;
	int num;
	int fd;
	char tmp[BUFLEN];
	int size = sizeof(id_table) / sizeof(id_table[0]); // 테이블 데이터 개수

	if((fd = creat("score.csv", 0666)) < 0){
		fprintf(stderr, "creat error for score.csv");
		return;
	}
	write_first_row(fd); // 테이블 제목 행 생성(문제 번호)

	for(num = 0; num < size; num++)
	{
		if(!strcmp(id_table[num], "")) // 학생 테이블 내용이 존재하지 않을 경우
			break;

		sprintf(tmp, "%s,", id_table[num]); // tmp = 2020XXXX
		write(fd, tmp, strlen(tmp));  // score.csv -> 2020xxxx,

		score += score_student(fd, id_table[num]); // 학생의 점수 계산
	}

	printf("Total average : %.2f\n", score / num);

	close(fd);
}

double score_student(int fd, char *id) // 학생의 점수 계산
{
	int type;
	double result;
	double score = 0;
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);

	for(i = 0; i < size ; i++)
	{
		if(score_table[i].score == 0)
			break;

		sprintf(tmp, "%s/%s/%s", stuDir, id, score_table[i].qname); // STD_DIR/2020XXXX/X-X.txt | STD_DIR/2020XXXX/X.c

		if(access(tmp, F_OK) < 0) // 학생 답안 파일 정보 가져오기
			result = false;
		else
		{
			if((type = get_file_type(score_table[i].qname)) < 0) 
				continue;
			
			if(type == TEXTFILE) // 빈칸 문제(X-X.txt)
				result = score_blank(id, score_table[i].qname);
			else if(type == CFILE) // 프로그램 문제(X.c)
				result = score_program(id, score_table[i].qname);
		}

		if(result == false) 
			write(fd, "0,", 2);
		else{
			if(result == true){
				score += score_table[i].score;
				sprintf(tmp, "%.2f,", score_table[i].score);
			}
			else if(result < 0){
				score = score + score_table[i].score + result;
				sprintf(tmp, "%.2f,", score_table[i].score + result);
			}
			write(fd, tmp, strlen(tmp));
		}
	}

	printf("%s is finished.. score : %.2f\n", id, score); 

	sprintf(tmp, "%.2f\n", score);
	write(fd, tmp, strlen(tmp));

	return score;
}

void write_first_row(int fd)
{
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);

	write(fd, ",", 1);

	for(i = 0; i < size; i++){
		if(score_table[i].score == 0)
			break;
		
		sprintf(tmp, "%s,", score_table[i].qname);
		write(fd, tmp, strlen(tmp));
	}
	write(fd, "sum\n", 4);
}

char *get_answer(int fd, char *result)
{
	char c;
	int idx = 0;

	memset(result, 0, BUFLEN);
	while(read(fd, &c, 1) > 0)
	{
		if(c == ':')
			break;
		
		result[idx++] = c;
	}
	if(result[strlen(result) - 1] == '\n')
		result[strlen(result) - 1] = '\0';

	return result;
}

int score_blank(char *id, char *filename) // 빈칸 문제 채점
{
	char tokens[TOKEN_CNT][MINLEN];
	node *std_root = NULL, *ans_root = NULL;
	int idx, start;
	char tmp[BUFLEN];
	char s_answer[BUFLEN], a_answer[BUFLEN];
	char qname[FILELEN] = { 0 };
	int fd_std, fd_ans;
	int result = true;
	int has_semicolon = false;

	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));

	// 학생 빈칸 문제 답안
	sprintf(tmp, "%s/%s/%s", stuDir, id, filename); // STD_DIR/2020XXXX/X-X.txt
	fd_std = open(tmp, O_RDONLY);
	strcpy(s_answer, get_answer(fd_std, s_answer));

	if(!strcmp(s_answer, "")){
		close(fd_std);
		return false;
	}

	if(!check_brackets(s_answer)){
		close(fd_std);
		return false;
	}

	strcpy(s_answer, ltrim(rtrim(s_answer)));

	if(s_answer[strlen(s_answer) - 1] == ';'){
		has_semicolon = true;
		s_answer[strlen(s_answer) - 1] = '\0';
	}

	if(!make_tokens(s_answer, tokens)){
		close(fd_std);
		return false;
	}

	idx = 0;
	std_root = make_tree(std_root, tokens, &idx, 0);

	// 정답 빈칸 문제
	sprintf(tmp, "%s/%s", ansDir, filename); // ANS_DIR/X-X.txt 
	fd_ans = open(tmp, O_RDONLY);

	while(1)
	{
		ans_root = NULL;
		result = true;

		for(idx = 0; idx < TOKEN_CNT; idx++)
			memset(tokens[idx], 0, sizeof(tokens[idx]));

		strcpy(a_answer, get_answer(fd_ans, a_answer));

		if(!strcmp(a_answer, ""))
			break;

		strcpy(a_answer, ltrim(rtrim(a_answer)));

		if(has_semicolon == false){
			if(a_answer[strlen(a_answer) -1] == ';')
				continue;
		}

		else if(has_semicolon == true)
		{
			if(a_answer[strlen(a_answer) - 1] != ';')
				continue;
			else
				a_answer[strlen(a_answer) - 1] = '\0';
		}

		if(!make_tokens(a_answer, tokens))
			continue;

		idx = 0;
		ans_root = make_tree(ans_root, tokens, &idx, 0);

		compare_tree(std_root, ans_root, &result);

		if(result == true){
			close(fd_std);
			close(fd_ans);

			if(std_root != NULL)
				free_node(std_root);
			if(ans_root != NULL)
				free_node(ans_root);
			return true;

		}
	}
	
	close(fd_std);
	close(fd_ans);

	if(std_root != NULL)
		free_node(std_root);
	if(ans_root != NULL)
		free_node(ans_root);

	return false;
}

double score_program(char *id, char *filename)
{
	double compile;
	int result;

	compile = compile_program(id, filename);

	if(compile == ERROR || compile == false)
		return false;
	
	result = execute_program(id, filename);

	if(!result)
		return false;

	if(compile < 0)
		return compile;

	return true;
}

int is_thread(char *qname)
{
	int i;
	int size = sizeof(threadFiles) / sizeof(threadFiles[0]);

	for(i = 0; i < size; i++){
		if(!strcmp(threadFiles[i], qname))
			return true;
	}
	return false;
}

double compile_program(char *id, char *filename) // 프로그램 문제 컴파일
{
	int fd;
	char tmp_f[BUFLEN], tmp_e[BUFLEN];
	char command[BUFLEN];
	char qname[FILELEN] = { 0 };
	int isthread;
	off_t size;
	double result;

	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.'))); // qname = X
	
	isthread = is_thread(qname);

	// 정답 프로그램 컴파일
	sprintf(tmp_f, "%s/%s", ansDir, filename); // ANS_DIR/X.c
	sprintf(tmp_e, "%s/%s.exe", ansDir, qname); // ANS_DIR/X.exe

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);

	sprintf(tmp_e, "%s/%s_error.txt", ansDir, qname);
	fd = creat(tmp_e, 0666);

	redirection(command, fd, STDERR);
	size = lseek(fd, 0, SEEK_END);
	close(fd);
	unlink(tmp_e);

	if(size > 0)
		return false;

	// 학생 답안 프로그램 컴파일
	sprintf(tmp_f, "%s/%s/%s", stuDir, id, filename); // STD_DIR/2020XXXX/X.c
	sprintf(tmp_e, "%s/%s/%s.stdexe", stuDir, id, qname); // STD_DIR/2020XXXX/X.stdexe

	if(tOption && isthread)
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);

	sprintf(tmp_f, "%s/%s/%s_error.txt", stuDir, id, qname); // STD_DIR/2020XXXX/X_error.txt
	fd = creat(tmp_f, 0666);

	redirection(command, fd, STDERR);
	size = lseek(fd, 0, SEEK_END);
	close(fd);

	if(size > 0){
		if(eOption)
		{
			sprintf(tmp_e, "%s/%s", errorDir, id); // ERROR/2020XXXX
			if(access(tmp_e, F_OK) < 0)
				mkdir(tmp_e, 0755);

			sprintf(tmp_e, "%s/%s/%s_error.txt", errorDir, id, qname);
			rename(tmp_f, tmp_e);

			result = check_error_warning(tmp_e);
		}
		else{ 
			result = check_error_warning(tmp_f);
			unlink(tmp_f);
		}

		return result;
	}

	unlink(tmp_f);
	return true;
}

double check_error_warning(char *filename)
{
	FILE *fp;
	char tmp[BUFLEN];
	double warning = 0;

	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "fopen error for %s\n", filename);
		return false;
	}

	while(fscanf(fp, "%s", tmp) > 0){
		if(!strcmp(tmp, "error:"))
			return ERROR;
		else if(!strcmp(tmp, "warning:"))
			warning += WARNING;
	}

	return warning;
}

int execute_program(char *id, char *filename)
{
	char std_fname[BUFLEN], ans_fname[BUFLEN];
	char tmp[BUFLEN];
	char qname[FILELEN];
	time_t start, end;
	pid_t pid;
	int fd;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));

	// 정답 프로그램 실행
	sprintf(ans_fname, "%s/%s.stdout", ansDir, qname); // ANS_DIR/X.stdout
	fd = creat(ans_fname, 0666);

	sprintf(tmp, "%s/%s.exe", ansDir, qname); // ANS_DIR/X.exe
	redirection(tmp, fd, STDOUT);
	close(fd);

	// 학생 답안 프로그램 실행 
	sprintf(std_fname, "%s/%s/%s.stdout", stuDir, id, qname); // STD_DIR/2020XXXX/X.stdout
	fd = creat(std_fname, 0666);

	sprintf(tmp, "%s/%s/%s.stdexe &", stuDir, id, qname); // STD_DIR/2020XXXX/X.stdexe &

	start = time(NULL);
	redirection(tmp, fd, STDOUT);
	
	sprintf(tmp, "%s.stdexe", qname); // X.stdexe
	while((pid = inBackground(tmp)) > 0){
		end = time(NULL);

		if(difftime(end, start) > OVER){
			kill(pid, SIGKILL);
			close(fd);
			return false;
		}
	}

	close(fd);

	return compare_resultfile(std_fname, ans_fname);
}

pid_t inBackground(char *name)
{
	pid_t pid;
	char command[64];
	char tmp[64];
	int fd;
	off_t size;
	
	memset(tmp, 0, sizeof(tmp));
	fd = open("background.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

	sprintf(command, "ps | grep %s", name);
	redirection(command, fd, STDOUT);

	lseek(fd, 0, SEEK_SET);
	read(fd, tmp, sizeof(tmp));

	if(!strcmp(tmp, "")){
		unlink("background.txt");
		close(fd);
		return 0;
	}

	pid = atoi(strtok(tmp, " "));
	close(fd);

	unlink("background.txt");
	return pid;
}

int compare_resultfile(char *file1, char *file2)
{
	int fd1, fd2;
	char c1, c2;
	int len1, len2;

	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);

	while(1)
	{
		while((len1 = read(fd1, &c1, 1)) > 0){
			if(c1 == ' ') 
				continue;
			else 
				break;
		}
		while((len2 = read(fd2, &c2, 1)) > 0){
			if(c2 == ' ') 
				continue;
			else 
				break;
		}
		
		if(len1 == 0 && len2 == 0)
			break;

		to_lower_case(&c1);
		to_lower_case(&c2);

		if(c1 != c2){
			close(fd1);
			close(fd2);
			return false;
		}
	}
	close(fd1);
	close(fd2);
	return true;
}

void redirection(char *command, int new, int old) // 출력 디스크럽터 변경
{
	int saved;

	saved = dup(old);
	dup2(new, old);

	system(command);

	dup2(saved, old);
	close(saved);
}

int get_file_type(char *filename) // 파일 유형 리턴
{
	char *extension = strrchr(filename, '.');

	if(!strcmp(extension, ".txt"))
		return TEXTFILE;
	else if (!strcmp(extension, ".c"))
		return CFILE;
	else
		return -1;
}

void rmdirs(const char *path)
{
	struct dirent *dirp;
	struct stat statbuf;
	DIR *dp;
	char tmp[BUFLEN] = { 0 };
	
	if((dp = opendir(path)) == NULL)
		return;

	// 해당 디렉토리 안의 내용물을 전부 제거
	while((dirp = readdir(dp)) != NULL)
	{
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name);

		if(lstat(tmp, &statbuf) == -1)
			continue;

		if(S_ISDIR(statbuf.st_mode)) // 디렉토리일 경우 재귀적으로 제거 ($rm -rf)
			rmdirs(tmp);
		else
			unlink(tmp);
	}

	closedir(dp);
	rmdir(path);
}

void to_lower_case(char *c) // 대문자를 소문자로 변환
{
	if(*c >= 'A' && *c <= 'Z')
		*c = *c + 32;
}

void print_usage()
{
	printf("Usage : ssu_score <STUDENTDIR> <TRUEDIR> [OPTION]\n");
	printf("Option : \n");
	printf(" -m                modify question's score\n");
	printf(" -e <DIRNAME>      print error on 'DIRNAME/ID/qname_error.txt' file \n");
	printf(" -t <QNAMES>       compile QNAME.C with -lpthread option\n");
	printf(" -i <IDS>          print ID's wrong questions\n");
	printf(" -h                print usage\n");
}
