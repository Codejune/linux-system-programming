#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED 1024

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

char *pathname;
char command[LINE_MAX], grep_cmd[LINE_MAX];

int ssu_do_grep(void){
	struct dirent *dirp;
	struct stat statbuf;
	char *ptr;
	DIR *dp;

	if(lstat(pathname,&statbuf) < 0){ // 파일의 상태정보를 가져옴
		fprintf(stderr,"lstat error for %s\n",pathname);
		return 0;
	}

	if(S_ISDIR(statbuf.st_mode) ==0){ //디렉토리가 아닌 경우(일반 파일)
		sprintf(command,"%s %s",grep_cmd,pathname);
		printf("%s : \n",pathname);
		system(command); // grep 명령어로 내용 탐색
		return 0;
	}

	ptr = pathname + strlen(pathname);
	*ptr++ = '/';
	*ptr = '\0';//pathname의 마지막에 /를 추가함

	if((dp = opendir(pathname))==NULL) { // pathname에 대한 DIR 구조체 획득
		fprintf(stderr,"opendir error for %s\n",pathname);
		return 0;
	}

	while((dirp = readdir(dp)) != NULL)
		if(strcmp(dirp->d_name,".") && strcmp(dirp->d_name,"..")){ //읽은 파일이나 디렉토리가 자신의 디렉토리인 "."이나 부모 디렉토리인 ".."이 아닌 겨우
			strcpy(ptr,dirp->d_name); //ptr에 파일 이름 저장

			if(ssu_do_grep() < 0) // 에러 발생시
				break;
		}


	ptr[-1] = 0;
	closedir(dp);
	return 0;
}

void ssu_make_grep(int argc, char *argv[]) { // grep 명령어 문자열 생성
	int i;
	strcpy(grep_cmd,"grep"); // 명령어 추가

	for(i=1;i<argc-1;i++){
		strcat(grep_cmd," "); // 공백 추가
		strcat(grep_cmd,argv[i]); // 인자 추가
	}
}

int main(int argc, char *argv[])
	{
		if(argc<2){
			fprintf(stderr,"usage: %s <-CVbchilnsvwx> <-num> <-A num> <-B num> <-f file> \n"                               "          <-e> expr <directory>\n",argv[0]);
			exit(1);
		}

		if(pathmax == 0) {
			if((pathmax = pathconf("/",_PC_PATH_MAX))<0)
				pathmax = MAX_PATH_GUESSED;
			else
				pathmax++;
		}

		if((pathname = (char *) malloc(pathmax+1)) == NULL) {
			fprintf(stderr,"malloc error\n");
			exit(1);
		}

		strcpy(pathname,argv[argc-1]);//마지막 인자를 pathname에 복사함
		ssu_make_grep(argc,argv);//grep 명령어 형태로 만듬
		ssu_do_grep();
		exit(0);
	}
