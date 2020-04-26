#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(intargc, char *argv[])
{
  pid_t pid;
  char character, first, last;
  long i;

  if ((pid = fork()) > 0) { // 1. 부모(대문자)가 자식 프로세스 호출
    first = 'A';
    last = 'Z';
  }
  else if (pid == 0) { // 2. 자식(소문자)
    first = 'a';
    last = 'z';
  }
  else {
    fprintf(stderr, "%s\n", argv[0]);
    exit(1);
  }

  for (character = first; character <= last; character++) {
    for (i= 0; i <= 100000; i++); // 대기
    write(1, &character, 1); // 문자 출력
  }

  printf("\n");
  exit(0);
}
