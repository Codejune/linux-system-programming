#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EXIT_CODE 1

int main(void)
{
  pid_t pid;
  int ret_val, status;

  if ((pid = fork()) == 0) { // 1. 자식 프로세스 호출
    // 2-1. 자식 프로세스 출력
    printf("child: pid= %d ppid= %d exit_code= %d\n", getpid(), getppid(), EXIT_CODE);
    exit(EXIT_CODE); // 3. 자식 프로세스 종료
  }

  // 2-2. 부모 프로세스 출력
  printf("parent: waiting for child = %d\n", pid);
  ret_val = wait(&status); // 종료 상태 대기 및 상태값 저장

  printf("parent: return value = %d, ", ret_val);
  printf(" child's status = %x", status);
  printf(" and shifted = %x\n", (status >> 8));
  exit(0);
}
