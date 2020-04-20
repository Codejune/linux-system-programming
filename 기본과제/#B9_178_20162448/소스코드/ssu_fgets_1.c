#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    while (fgets(buf, BUFFER_SIZE, stdin) != NULL) // 문자를 버퍼에 입력받고
        if (fputs(buf, stdout) == EOF) { // 표준 출력에 출력
            fprintf(stderr, "standard output error\n");
            exit(1);
        }
    if (ferror(stdin)) { // 에러가 존재시
        fprintf(stderr, "standard input error\n");
        exit(1);
    }
    exit(0);
}
