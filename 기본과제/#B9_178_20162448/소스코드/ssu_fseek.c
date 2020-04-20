#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *fname = "ssu_test.txt";
    FILE *fp;
    long position;
    int character;
    
    if ((fp = fopen(fname, "r")) == NULL) { // 파일 읽기 전용 열기
        fprintf(stderr, "fopen error for %s\n", fname);
        exit(1);
    }
    printf("Input number >>");
    scanf("%ld", &position); // 숫자 입력
    fseek(fp, position - 1, SEEK_SET); // 해당 수만큼 오프셋 이동
    character = getc(fp); // 해당 오프셋 위치 문자 읽음
    printf("%ldth character => %c\n", position, character);
    exit(0);
}
