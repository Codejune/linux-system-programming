#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);
extern char **environ;
char glob_var[] = "HOBBY=swimming";

int main(void)
{
  int i;
  for (i= 0; environ[i] != NULL; i++) // 등록된 환경 변수 모두 출력
    printf("environ[%d] : %s\n", i, environ[i]);

    putenv(glob_var); // 환경 변수에 HOBBY 등록
    ssu_addone(); // 지역변수를 등록시 소멸할때 제대로 등록되지 않는다.
    printf("My hobby is %s\n", getenv("HOBBY")); // 환경변수에 등록된 HOBBY의 내용을 가져옴
    printf("My lover is %s\n", getenv("LOVER")); // 환경변수에 등록된 LOVER의 내용을 가져옴
    strcpy(glob_var+ 6, "fishing");
    for (i= 0; environ[i] != NULL; i++)
      printf("environ[%d] : %s\n", i, environ[i]);

    exit(0);
}

void ssu_addone(void)
{
  char auto_var[10];
  strcpy(auto_var, "LOVER=js");
  putenv(auto_var);
}
