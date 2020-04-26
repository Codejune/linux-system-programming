#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void ssu_charatatime(char *str);

int main(void) {
  pid_t pid;

  if ((pid = fork()) < 0) { // 1. 자식 프로세스 호출
    fprintf(stderr, "fork error\n");
    exit(1);
  }
  // 거의 동시 실행
  else if (pid == 0) // 2-1. 자식 프로세스 실행
    ssu_charatatime("output from child\n");
  else // 2-2. 부모 프로세스 실행
    ssu_charatatime("output from parent\n");

  exit(0);
}

static void ssu_charatatime(char *str) {
   char *ptr;
   int print_char;

   setbuf(stdout, NULL); // 표준출력으로 전환
   for (ptr= str; (print_char= *ptr++) != 0;) {
     putc(print_char, stdout); // 한글자씩 출력
     usleep(10);
   }
}
