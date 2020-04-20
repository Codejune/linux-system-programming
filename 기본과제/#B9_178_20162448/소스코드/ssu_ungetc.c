#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main(void)
{
    char operator;
    FILE *fp;
    int character;
    int number = 0;
    if ((fp = fopen("ssu_expr.txt", "r")) == NULL) { // 읽기 전용 열기
        fprintf(stderr, "fopen error for ssu_expr.txt\n");
        exit(1);
        
    }
    while (!feof(fp)) { // 파일의 끝에 도달할때까지
        while ((character = fgetc(fp)) != EOF && isdigit(character)) // EOF가 아니고, 숫자라면
            number = 10 * number + character – 48; // 자리수만큼의 숫자 변환
        fprintf(stdout, " %d\n", number);
        number = 0;
        if (character != EOF) { // 파일의 끝이 아니라면
            ungetc(character, fp); // 문자를 읽고 다시 스트림에 되돌려 둠
            operator = fgetc(fp);
            printf("Operator => %c\n", operator);
        }
    }
    fclose(fp); // 파일 닫음
    exit(0);
}
