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

  if ((pid = fork()) == 0) {
    printf("child: pid= %d ppid= %d exit_code= %d\n", getpid(), getppid(), EXIT_CODE);
    exit(EXIT_CODE);
  }

  printf("parent: waiting for child = %d\n", pid);
  ret_val = wait(&status);

  printf("parent: return value = %d, ", ret_val);
  printf(" child's status = %x", status);
  printf(" and shifted = %x\n", (status >> 8));
  exit(0);
}
