#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
  pid_t child1, child2;
  int pid, status;

  if ((child1 = fork()) == 0) // 1. 자식 프로세스 호출(c1)
    execlp("date", "date", (char *)0);

  if ((child2 = fork()) == 0) // 2. 자식 프로세스 호출(c2)
    execlp("who", "who", (char *)0); // 5. c1 출력

  printf("parent: waiting for children\n"); // 3. 부모 프로세스 출력

  while ((pid= wait(&status)) != -1) { // 4. 부모 프로세스 대기
    if (child1 == pid) // 7. c1 후 종료
      printf("parent: first child: %d\n", (status >> 8));
    else if (child2 == pid) // 6. c2 선 종료
      printf("parent: second child: %d\n", (status >> 8));
  }

  printf("parent: all children terminated\n");
  exit(0);
}
