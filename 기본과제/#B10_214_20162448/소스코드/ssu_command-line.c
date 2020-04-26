#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  int i;
  for (i= 0; i< argc; i++) // 인자 개수 만큼
    printf("argv[%d] : %s\n", i, argv[i]);  // 출력
    exit(0);
  }
