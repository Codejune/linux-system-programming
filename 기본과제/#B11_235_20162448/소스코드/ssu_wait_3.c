#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
  if (fork() == 0)
    execl("/bin/echo", "echo", "this is", "message one", (char *)0);

  if (fork() == 0)
    execl("/bin/echo", "echo", "this is", "message Two", (char *)0);

  printf("parent: waiting for children\n");

  while (wait((int*)0) != -1);

  printf("parent: all children terminated\n");
  exit(0);
}
