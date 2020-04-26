#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
  if (fork() == 0) // 1. 자식 프로세스 호출
    execl("/bin/echo", "echo", "this is", "message one", (char *)0); // 2-1. 자식 프로세스 출력

  if (fork() == 0) // 3. 자식 프로세스 출력
    execl("/bin/echo", "echo", "this is", "message Two", (char *)0);

  // 2-2. 부모 프로세스 출력
  printf("parent: waiting for children\n");

  // 2-3. 부모 프로세스 대기
  while (wait((int*)0) != -1);

  printf("parent: all children terminated\n");
  exit(0);
}
