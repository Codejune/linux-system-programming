#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char glob_str[] = "write to standard output\n";
int glob_val= 10;

int main(void)
{
  pid_t pid;
  int loc_val;

  loc_val= 100;

  if (write(STDOUT_FILENO, glob_str, sizeof(glob_str)-1) != sizeof(glob_str) -1) {  // 표준출력으로 glob_str 출력
    fprintf(stderr, "write error\n");
    exit(1);
  }

  printf("before fork\n"); // 포킹 전

  if ((pid = fork()) < 0) {  // 1. 자식의 pid 리턴 및 자식 프로세스 호출
    fprintf(stderr, "fork error\n");
    exit(1);
  }
  else if (pid == 0) { // 2. 자식 프로세스는 0을 리턴
    glob_val++;
    loc_val++;
  }
  else
    sleep(3);

  printf("pid = %d, glob_val = %d, loc_val = %d\n", getpid(), glob_val, loc_val); // 3. 자식 프로세스 출력, 4. 부모 프로세스 출력
  exit(0); 
}
