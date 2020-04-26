#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  printf("Good afternoon");
  exit(0); // 종료 처리부들이나 시그널핸들러들을 실행하고 최종적으로 _exit()를 호출
}
