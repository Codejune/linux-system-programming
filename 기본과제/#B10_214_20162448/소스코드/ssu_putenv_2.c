#include <stdio.h>
#include <stdlib.h>

void ssu_printenv(char *label, char ***envpp);
extern char **environ;

int main(int argc, char *argv[], char *envp[]) // 3번째 인자 = 환경변수
{
  ssu_printenv("Initially", &envp); // 등록 전 출력
  putenv("TZ=PST8PDT"); // 등록 1
  ssu_printenv("After changing TZ", &envp); // 등록 1 후 출력
  putenv("WARNING=Don't use envpafter putenv()"); // 등록 2
  ssu_printenv("After setting a new variable", &envp); // 등록 2 후 출력
  printf("value of WARNING is %s\n", getenv("WARNING")); // WARNING 환경변수 불러오기 및 출력
  exit(0);
}

void ssu_printenv(char *label, char ***envpp)
{
  char **ptr;
  printf("----%s ---\n", label);
  printf("envpis at %8o and contains %8o\n", envpp, *envpp);
  printf("environ is at %8o and contains %8o\n", &environ, environ);
  printf("My environment variable are:\n");
  for (ptr = environ; *ptr; ptr++)
    printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr); // 등록된 환경 변수 출력
  printf("(%8o) = %8o\n", ptr, *ptr); // 끝지점 출력
}
