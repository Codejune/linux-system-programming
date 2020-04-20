#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    char name[L_tmpnam];
    FILE *fp;
    printf("Temporary filename <<%s>>\n", tmpnam(name)); // 임시 파일 생성 1
    if ((fp = tmpfile()) == NULL) { // 임시 파일 열기
        fprintf(stderr, "tmpfile create error!!\n");
        exit(1);
    }
    fputs("create tmpfile success!!\n", fp); // 파일에 내용 쓰기
    rewind(fp); // 오프셋 맨 처음으로 이동
    fgets(buf, sizeof(buf), fp); // 파일에 쓰여진 길이만큼 읽기
    puts(buf); // 표준 출력에 출력
    exit(0);
}
