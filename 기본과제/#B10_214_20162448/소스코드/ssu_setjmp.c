#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register);
static jmp_buf glob_buffer;

int main(void)
{
  register int loc_register; // 레지스터에 변수 공간 할당
  volatile int loc_volatile; // 메모리에 변수 공간 할당
  int loc_var;

  // 변수 지정
  loc_var= 10;
  loc_volatile= 11;
  loc_register= 12;

  if (setjmp(glob_buffer) != 0) { // 직접 호출시 0, LONGJMP에 의해 호출시 이외의 값, 점프시 환경 내용을 env변수에 저장
    printf("after longjmp, loc_var= %d, loc_volatile= %d, loc_register= %d\n", loc_var, loc_volatile, loc_register);
    exit(0);
  }

  // 변수 재지정
  loc_var= 80;
  loc_volatile= 81;
  loc_register= 83;

  ssu_nested_func(loc_var, loc_volatile, loc_register);
  exit(0);
}

void ssu_nested_func(int loc_var, int loc_volatile, int loc_register)
{
  printf("before longjmp, loc_var= %d, loc_volatile= %d, loc_register= %d\n", loc_var, loc_volatile, loc_register);
  longjmp(glob_buffer, 1); // setjmp 지점으로 점프, 두번째 인자는 점프한 위치의 setjmp가 리턴할 값
}
