#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;
	char *fname1 = "ssu_file1";
	char *fname2 = "ssu_file2";

	if(stat(fname1, &statbuf) < 0) // ssu_file1의 정보를 stat()을 통해 가져옴
		fprintf(stderr, "stat error %s\n", fname1);

	if(chmod(fname1, (statbuf.st_mode & ~S_IXGRP) | S_ISUID) < 0)  // ssu_file1의 권한에서 그룹의 실행권한을 빼고, UID 추가
		fprintf(stderr, "chmod error %s\n", fname1);
	
	if(chmod(fname2, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH) < 0) // ssu_file2에 rw_r__rw_권한 추가
		fprintf(stderr, "chmod error %s\n", fname2);

	exit(0); // 정상 종료
}
