#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 4096

int main(void)
{
    char buf[MAX_LINE];
    char name[L_tmpnam];
    FILE *fp;
    printf("temp file 1 : %s\n", tmpnam(NULL)); // 임시 파일 생성 1
    tmpnam(name);
    printf("temp file 2 : %s\n", name); // 임시 파일 생성 2
    if ((fp = tmpfile()) == NULL) { // 임시파일 읽기 전용 열기
        fprintf(stderr, "tmpfile error\n");
        exit(1);
    }
    fputs("tmpfile created temporary file.\n", fp); // 파일에 내용 쓰기
    fseek(fp, 0, SEEK_SET); // 오프셋 맨 처음으로 이동
    if (fgets(buf, sizeof(buf), fp) == NULL) { // 파일에 쓰여진 내용 길이 만큼 읽음
        fprintf(stderr, "fgets error\n");
        exit(1);
    }
    fputs(buf, stdout); // 표준 출력에 출력
    exit(0);
}
