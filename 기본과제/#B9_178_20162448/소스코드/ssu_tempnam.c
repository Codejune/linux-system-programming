#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    char *arg_directory = NULL; // 임시 파일 생성 경로
    char *arg_prefix = NULL; // 임시 파일 규칙 이름
    if (argc != 3) { // 인자 개수가 부족할 경우
        fprintf(stderr, "usage: %s <directory> <prefix>\n", argv[0]);
        exit(1);
    }
    arg_directory = argv[1][0] != ' ' ? argv[1] : NULL; // 공백이 아니면 argv[1], 맞으면 NULL
    arg_prefix = argv[2][0] != ' ' ? argv[2] : NULL; // 공백이 아니면 argv[2], 맞으면 NULL
    printf("created : %s\n", tempnam(arg_directory, arg_prefix)); // 디렉토리 경로 지정 및 임시파일 생성
    exit(0); }
