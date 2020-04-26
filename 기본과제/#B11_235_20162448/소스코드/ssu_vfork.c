#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

void ssu_local_func(void);

int main(void)
{
  printf("Before vfork\n");
  ssu_local_func();
  printf("After ssu_local_func, my PID is %d\n", getpid()); // 3. 자식 출력, 4. 부모 출력
  exit(0); // 자식이 부모와 공유하고 있던 입출력 스트림을 모두 닫아서 종료됨 PID=0
}

void ssu_local_func(void)
{
  pid_t pid;
  if ((pid = vfork()) == 0) // 1. 자식이 먼저 실행
    printf("I'm child. My PID is %d\n", getpid());
  else if (pid> 0) // 2. 부모는 대기
    sleep(3);
  else
   fprintf(stderr, "vfork error\n");
}
