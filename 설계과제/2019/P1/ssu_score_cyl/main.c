#include "ssu_score.h"

int main(int argc,char *argv[])
{
	gettimeofday(&begin,NULL); // 시작 시간
	input_check(argc,argv); // 입력값 확인
	before_grading(); // 채점 준비
	grading(); // 채점
	make_result(); // 채점 결과 저장
	if(options.opt_c)
		option_c(FALSE);
	gettimeofday(&end,NULL); // 끝난 시간
	ssu_runtime(&begin,&end); // 실행시간 출력
	exit(EXIT_SUCCESS);
}
