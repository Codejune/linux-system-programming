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
		if(stat(argv[i],&statbuf)<0){//인자에 대한 stat 구조체 획득
			fprintf(stderr,"stat error for %s\n",argv[i]);
			continue;
		}

		if((fd = open(argv[i],O_RDWR|O_TRUNC))<0){//파일 오픈(atime과mtime 변경)
				fprintf(stderr,"open error for %s\n",argv[i]);
				continue;
				}

		close(fd);

		time_buf.actime = statbuf.st_atime;//open() 호출 전 시간을 저장
		time_buf.modtime = statbuf.st_mtime;//open() 호출 전 시간을 저장

		if(utime(argv[i],&time_buf)<0){//argv[i]의 atime과 mtime을 위에 저장해둔 값으로 변경
			fprintf(stderr,"utime error for %s\n",argv[i]);
			continue;
		}

	}
		exit(0);
}
