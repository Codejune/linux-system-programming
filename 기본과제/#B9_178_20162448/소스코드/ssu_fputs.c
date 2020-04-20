#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main (int argc, char *argv[])
{
    char buf[BUFFER_SIZE];
    FILE *fp;
    
    if (argc != 2) { // 인자 개수가 부족할 경우
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(1);
    }
    
    if ((fp = fopen(argv[1], "w+")) == NULL) { // 쓰기 전용, 존재시 덮어쓰기로 열기
        fprintf(stderr, "fopen error for %s\n", argv[1]);
        exit(1);
    }
    
    fputs("Input String >> ", stdout); // 표준 출력에 해당 내용 출력
    gets(buf); // 버퍼에 내용을 입력받음
    fputs(buf, fp); // 버퍼의 내용을 파일에 쓰기
    rewind(fp); // 오프셋을 맨 처음으로 이동
    fgets(buf, sizeof(buf), fp); // 버퍼에 쓰여진 내용의 길이만큼 파일에서 읽음
    puts(buf); // 표준 출력에 출력
    fclose(fp); // 파일 닫음
    exit(0);
}
