#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char *argv[])
{
	struct utimbuf time_buf;
	struct stat statbuf;
	int fd;
	int i;

	for(i = 1;i < argc;i++)
	{
		if(stat(argv[i],&statbuf)<0){ // open() 호출 전 인자로 받은 파일명에 대한 stat 구조체 획득
			fprintf(stderr,"stat error for %s\n",argv[i]);
			continue;
		}

		if((fd = open(argv[i],O_RDWR|O_TRUNC))<0){ // 파일 열기(읽기쓰기모드, 존재시 다시쓰기), atime, mtime 변경
				fprintf(stderr,"open error for %s\n",argv[i]);
				continue;
				}

		close(fd);

		time_buf.actime = statbuf.st_atime; // open() 최종 접근 시간 저장
		time_buf.modtime = statbuf.st_mtime; // open() 최종 수정 시간 저장

		if(utime(argv[i],&time_buf) < 0){ // 인자로 주어진 파일의 atime과 mtime을 위에 저장해둔 값으로 변경
			fprintf(stderr,"utime error for %s\n",argv[i]);
			continue;
		}

	}
		exit(0);
}
