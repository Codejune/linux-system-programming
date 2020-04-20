#include <stdio.h>
#include <stdlib.h>
#define BUFFER_MAX 256

int main(void)
{
    char command[BUFFER_MAX];
    char *prompt = "Prompt>>";

    while (1) {
        fputs(prompt, stdout); // 표준 출력에 출력
        if (fgets(command, sizeof(command), stdin) == NULL) // 버퍼에 내용 입력받고, 내용이 없으면 탈출
            break;
        system(command); // 입력받은 내용 실행
    }
    fprintf(stdout, "Good bye...\n");
    fflush(stdout); // 표준 출력 스트림을 비움
    exit(0);
}
