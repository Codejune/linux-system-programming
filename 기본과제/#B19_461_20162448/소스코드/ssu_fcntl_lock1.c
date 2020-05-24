#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <errno.h>

#define NAMESIZE 50
#define MAXTRIES 5

struct employee {
  char name[NAMESIZE];
  int salary;
  int pid;
};

int main(int argc, char *argv[])
{
  struct flock lock;
  struct employee record;
  int fd, sum = 0, try = 0;

  sleep(10);

  // 인자로 입력받은 파일 열기
  if((fd = open(argv[1], O_RDONLY)) == -1) {
    perror(argv[1]);
    exit(1);
  }

  // 락 상태 정보 초기화
  lock.l_type = F_RDLCK;
  lock.l_whence = 0;
  lock.l_start = 0L;
  lock.l_len = 0L;

  while(fcntl(fd, F_SETLK, &lock) == -1) { // read lock 설정
    if(errno == EACCES) { // 에러 발생시
      if(try++ < MAXTRIES) { // 5번 다시 시도
        sleep(1); // 1초 대기
        continue;
      }
      printf("%s busy -- try later\n", argv[1]);
      exit(2);
    }
    perror(argv[1]); // 에러 출력
    exit(3);
  }

  sum = 0;
  fseek(fd, 0, SEEK_SET);
  while(read(fd, (char*)&record, sizeof(record)) > 0) {
    printf("Employee: %s, Salary: %d\n", record.name, record.salary);
    sum += record.salary;
  }
  printf("\nTotal salary: %d\n", sum);

  // read lock 해제
  lock.l_type = F_UNLCK;
  fcntl(fd, F_SETLK, &lock);
  close(fd);
}
