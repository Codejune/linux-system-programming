#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char line[256];
	FILE *fp;
	int uid;
	if(argc<2){//인자입력확인
		fprintf(stderr,"usage : %s file_name\n",argv[0]);
		exit(1);
	}//에러처리
	printf("initially uid=%d and euid=%d\n",getuid(),geteuid());//초기 사용자아이디, 유효사용자아이디출력
	fp=fopen(argv[1],"r");//읽기권한으로 파일오픈
	if(fp==NULL){
		fprintf(stderr,"first open error for %s\n",argv[1]);
		exit(1);
	}//파일오픈에러
	else{//오픈에 성공했다면 읽은내용 표준출력에 출력 후 파일 닫음
		printf("first open successful:\n");
		while(fgets(line,255,fp)!=NULL)
			fputs(line,stdout);
		fclose(fp);
	}
	setuid(uid=getuid());//아이디바꿈
	printf("after setuid(%d):\n uid=%d and euid=%d\n",uid,getuid(),geteuid());
	fp=fopen(argv[1],"r");//파일 재오픈
	if(fp==NULL){
		fprintf(stderr,"second open error for %s\n",argv[1]);
		exit(1);
	}//오픈에러처리
	else{//오픈성공시 내용 출력
		printf("second open successful:\n");
		while(fgets(line,255,fp)!=NULL)
			fputs(line,stdout);
		fclose(fp);
	}
	exit(0);
}

