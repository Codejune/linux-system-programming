#include <stdio.h>
#include <stdlib.h>
void ssu_out(void);
int main(void)
{
  if (atexit(ssu_out)) { // exit전에 실행할 함수를 등록
    fprintf(stderr, "atexit error\n");
    exit(1); // ssu_out 실행 후 종료
  }
  exit(0); // ssu_out 실행 후 종료
}

void ssu_out(void)
{
   printf("atexit succeeded!\n");
 }
