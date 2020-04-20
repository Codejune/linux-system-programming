#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main(void)
{
    char *fname = "ssu_test.dat";
    char name[BUFFER_SIZE];
    FILE *fp;
    int age;
    
    fp = fopen(fname, "r"); // 파일 읽기 전용 열기
    fscanf(fp, "%s%d", name, &age); // 파일에서 문자열, 숫자 형식으로 데이터 읽어들임
    fclose(fp); // 파일 닫기
    fp = fopen(fname, "w"); // 파일 쓰기 전용 열기, 이미 존재하므로 덮어씀
    fprintf(fp, "%s is %d years old\n", name, age); // 파일에 쓰기
    fclose(fp); // 파일 닫기
    exit(0);
}
