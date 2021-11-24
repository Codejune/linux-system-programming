#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

void main()
{
    char *fname = "ssu_line.txt";
    char *frname= "ssu_blank.txt";
    int fd_w, fd_r;	        //쓰기 파일 디스크립터, 읽을 파일 디스크립터
    size_t length;	        //read()의 리턴값
    int wordcnt = 1;      //읽은 파일의 문자의 개수
    char buf[50];	        //read()에서 읽는 버퍼
    int i = 0;		//for(),while() 문에 조건문 사용

    printf("sorry...");
}
