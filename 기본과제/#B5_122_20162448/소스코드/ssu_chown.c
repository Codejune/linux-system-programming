#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;
	char *fname = "ssu_myfile"; // 생성할 파일 이름
	int fd;

	if((fd = open(fname, O_RDWR | O_CREAT, 0600)) < 0) { // ssu_myfile 파일 생성, 읽기 쓰기 모드, 권한 rw_______
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	close(fd); // 생성 후 닫기 
	stat(fname, &statbuf); // 해당 파일의 정보를 가져옴
	printf("# 1st stat call # UID:%d GID:%d\n", statbuf.st_uid, statbuf.st_gid);

	if(chown(fname, 501, 300) < 0) { // UID, GID -> 501, 300
		fprintf(stderr, "chown error for %s\n", fname);
		exit(1);
	}

	stat(fname, &statbuf); // 파일 상태 정보 갱신
	printf("# 2nd stat call # UID:%d GID:%d\n", statbuf.st_uid, statbuf.st_gid);

	if(unlink(fname) < 0) { // 생성된 파일 삭제
		fprintf(stderr, "unlink error for %s\n", fname);
		exit(1);
	}
	exit(0);
}
