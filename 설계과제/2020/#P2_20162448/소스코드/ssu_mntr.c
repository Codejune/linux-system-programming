#include "ssu_mntr.h"

int ssu_mntr(int argc, char *argv[])
{
	char command[256];
    char *prompt = "20162448>";

    while (1) {
        fputs(prompt, stdout); // 표준 출력에 출력
        if (fgets(command, sizeof(command), stdin) == NULL) // 버퍼에 내용 입력받고, 내용이 없으면 탈출
            print_usage();
    }
    fprintf(stdout, "Good bye...\n");
    fflush(stdout); // 표준 출력 스트림을 비움
    exit(0);
}

void print_usage(void) // 사용법 출력₩
{
	printf("Usage : [COMMAND]\n");
	printf("COMMAND : \n");
	printf(" DELETE [FILENAME] [END_TIME] [OPTION]      delete the file at the END_TIME\n");
	printf(" SIZE [FILENAME] [OPTION]                   print file path and size\n");
	printf(" RECOVER [FILENAME] [OPTION]                restore deleted files to their original path\n");
	printf(" TREE                                       print a list of monitoring files in a tree format.\n");
	printf(" EXIT                                       program exit\n");
	printf(" HELP                                       print usage\n");
}
