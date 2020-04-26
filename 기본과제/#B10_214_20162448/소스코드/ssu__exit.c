#include <stdio.h>
#include <unistd.h>
int main(void)
{
  printf("Good afternoon?");
  _exit(0); // 종료 처리부들이나 시그널처리부들을 실행하지않고 프로세스를 종료
}
