#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>

#define STDIN_VUMBER 0

int main(void){
	pid_t sid_stderr;
	pid_t sid_stdin;
	pid_t sid_stdout;

	sid_stdin = tcgetsid(STDIN_FILENO); // STDIN SID 획득

	if(sid_stdin == -1){ // 정상적인 획득이 아닐 경우
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session Leader for stdin: %d\n", sid_stdin);

	sid_stdout = tcgetsid(STDOUT_FILENO); // STDOUT SID 획득

	if(sid_stdout == -1){ // 정상적인 획득이 아닐 경우
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session Leader for stdout: %d\n", sid_stdout);


	sid_stderr = tcgetsid(STDERR_FILENO); // STDERR SID 획득

	if(sid_stderr == -1){ // 정상적인 획득이 아닐 경우
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session Leader for stderr: %d\n", sid_stderr);

	exit(0);
}
