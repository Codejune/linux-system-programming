#include "ssu_mntr.h"

extern char pwd[BUFFER_SIZE]; // ssu_mntr.h에서 공유
struct ssu_fileTable file_table[BUFFER_SIZE]; // 모니터링 파일 목록 테이블
int file_cnt = 0; // 모니터링 파일 개수

void mntr_process(void) // 모니터링 메인 함수
{
	FILE *fp;

	if((fp = fopen("log.txt", "w+")) == NULL) { // 로그 파일 열기 및 생성, 기존 로그 덮어쓰기
			fprintf(stderr, "fopen error for log.txt\n");
			return;
	}
	fclose(fp);

	set_fileTable(); // 모니터링 리스트 테이블 초기화
	printf("%d\n", file_cnt);
	while(true) { 
		check_change();
	}
}

void set_fileTable(void) // 모니터링 파일 목록 테이블 생성
{
	struct dirent *dirp; // $(PWD)/MONITORING_DIR 디렉토리 목록 포인터
	DIR *dp; // $(PWD)/check 디렉토리 포인터
	
	if(access(CHECK, F_OK) < 0) // 모니터링 디렉토리가 존재하지 않을 경우
		mkdir(CHECK, 0755); // 모니터링 디렉토리 생성


	if((dp = opendir(CHECK)) == NULL){ // $(PWD)/ANS_DIR 열기
		fprintf(stderr, "open dir error for check\n");
		return;
	}

	chdir(CHECK);

	while((dirp = readdir(dp)) != NULL) { // 모니터링 파일 목록 읽어오기

		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, "..")) // 현재, 상위 디렉토리 파일 접근 생략
			continue;

		strcpy(file_table[file_cnt].name, dirp->d_name); // 파일 이름 저장
		if(stat(dirp->d_name, &file_table[file_cnt++].status) < 0) { // 파일 상태 구조체 저장
			fprintf(stderr, "stat error for %s\n", dirp->d_name);
			return;
		}
	}

	closedir(dp);
	chdir(pwd);
}

void check_change(void)
{

}
