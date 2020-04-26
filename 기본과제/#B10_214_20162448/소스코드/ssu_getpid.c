#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main(void)
{
  printf("Process ID        = %d\n", getpid()); // 현재 PID
  printf("Parent process ID   = %d\n", getppid()); // 부모 PID
  printf("Real user ID        = %d\n", getuid()); // 현재 UID
  printf("Effective user ID   = %d\n", geteuid()); // 실효 UID
  printf("Real group ID       = %d\n", getgid()); // 현재 UID가 소속된 GID
  printf("Effective group ID  = %d\n", getegid()); // 실효 GID
  exit(0);
}
