#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE MAXNAMLEN // dirent.h에 정의되어 있는 이름 버퍼의 최대 크기

int main(int argc, char *argv[])
{
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE];
	DIR *dirp;

	if(argc < 2){ // 인자 개수 확인
		fprintf(stderr,"usage: %s <directory>\n",argv[0]);
		exit(1);
	}

	if((dirp = opendir(argv[1])) == NULL || chdir(argv[1]) == -1){ // 인자로 주어진 디렉토리이름을 통해 DIR 구조체 획득 및 현재 작업 디렉토리를 변경
		fprintf(stderr,"opendir, chdir for %s\n",argv[1]);
		exit(1);
	}

	while((dentry = readdir(dirp)) != NULL){ // DIR구조체 탐색
		if(dentry->d_ino == 0) // 디렉토리 아이노드 넘버
			continue;

		memcpy(filename, dentry->d_name,DIRECTORY_SIZE); // 디렉토리 내부에 존재하는 파일 이름들을 filename에 저장

		if(stat(filename,&statbuf)==-1){ // 해당 파일의 상태 정보를 가져옴
			fprintf(stderr,"stat error for %s\n",filename);
			exit(1);
		}

		if((statbuf.st_mode & S_IFMT)== S_IFREG) // 일반 파일일 경우
			printf("%-14s %ld\n",filename,statbuf.st_size);
		else // 아닐 경우
			printf("%-14s\n",filename);
	}
		exit(0);
}
