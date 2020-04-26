#include <stdio.h>
#include <stdlib.h>

static void ssu_exit1(void);
static void ssu_exit2(void);

int main(void)
{
  if (atexit(ssu_exit2) != 0) { // ssu_exit2 등록
    fprintf(stderr, "atexit error for ssu_exit2");
    exit(1);
  }

  if (atexit(ssu_exit1) != 0) { // ssu_exit1 등록
    fprintf(stderr, "atexit error for ssu_exit1");
    exit(1);
  }

  if (atexit(ssu_exit1) != 0) { // ssu_exit1 등록
    fprintf(stderr, "atexit error for ssu_exit1");
    exit(1);
  }

  printf("done\n");
  exit(0); // 등록된 순서의 역순으로 호출 -> 1, 1, 2 순으로 출력
}

static void ssu_exit1(void)
{
  printf("ssu_exit1 handler\n");
}

static void ssu_exit2(void)
{
  printf("ssu_exit2 handler\n");
}
