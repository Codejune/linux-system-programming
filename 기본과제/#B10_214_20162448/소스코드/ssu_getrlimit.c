#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#define ssu_show(limit) ssu_show_limit(limit, #limit)

void ssu_show_limit(int rlim_type, char *rlim_name);

int main(void)
{
  // 프로세스마다 적용된 자원 한계 값 출력
  printf("      resource soft limit hard limit       \n\n");
  ssu_show(RLIMIT_CPU); // 프로세스가 소비할 수 있는 최대 CPU 시간
  ssu_show(RLIMIT_FSIZE); // 파일 생성 시 파일의 최대 크기(바이트)
  ssu_show(RLIMIT_DATA); // 자료 구역 전체의 최대 크기(바이트)
  ssu_show(RLIMIT_STACK); // 스택 최대 크기(바이트)
  ssu_show(RLIMIT_CORE); // 한 코어 파일의 최대 크기(바이트)

#ifdef RLIMIT_RSS // 메모리 상주 세트 크기(Resident Set Size)의 최댓값(바이트)
  ssu_show(RLIMIT_RSS);
#endif
#ifdef RLIMIT_MEMLOCK // 한 프로세스가 mlock으로 잠글 수 있는 메모리의 최대 용량(바이트)
  ssu_show(RLIMIT_MEMLOCK);
#endif
#ifdef RLIMIT_NPROC // 실제 사용자 ID당 최대 자식 프로세스 개수
  ssu_show(RLIMIT_NPROC);
#endif
#ifdef RLIMIT_OFILE // RLIMIT_NOFILE에 대한 BSD 이름
  ssu_show(RLIMIT_OFILE);
#endif
#ifdef RLIMIT_NOFILE // 한 프로세스가 열어 둘 수 있는 최대 파일 개수
  ssu_show(RLIMIT_NOFILE);
#endif
#ifdef RLIMIT_VMEN // RLIMIT_AS와 동일, 한 프로세스의 총 자유 메모리 용량의 최댓값(바이트)
  ssu_show(RLIMIT_VMEN);
#endif
  exit(0);
}

/*
struct rlimit {
  rlim_t rlim_cur; // 약한 한계
  rlim_t rlim_max; // 강한 한계
}
*/

void ssu_show_limit(int rlim_type, char *rlim_name)
{
  struct rlimit rlim;
  char cur[11], max[11];

  getrlimit(rlim_type, &rlim);

  if(rlim.rlim_cur == RLIM_INFINITY) // 현재 잡힌 크기가 무한대의 경우
    strcpy(cur, "infinity");
  else
    sprintf(cur, "%10ld", rlim.rlim_cur);

  if(rlim.rlim_max == RLIM_INFINITY) // 최대 크기가 무한대의 경우
    strcpy(max, "infinity");
  else
    sprintf(max, "%10ld", rlim.rlim_max);

  printf("%15s : %10s %10s\n", rlim_name, cur, max); // 출력
}
