#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long fsize;
    
    if ((fp = fopen(fname, "r")) == NULL) { // 파일 읽기 전용 열기
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    fseek(fp, 0, SEEK_END); // 오프셋을 파일의 끝으로 이동
    fsize = ftell(fp); // 오프셋이 시작위치부터 얼마나 떨어졌는지 리턴
    printf("The size of <%s> is %ld bytes\n", fname, fsize);
    exit(0);
}
