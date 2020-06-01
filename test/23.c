#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256
#define STUDENT_NUM 3
#define Q_SIZE 10

typedef struct _student
{
	char name[10];
	int score;
	char res[BUFFER_SIZE];
} Student;

char answer[BUFFER_SIZE] = "1233423413"; // test's answer

int main(void)
{
	char *ssu_answer = "ssu_answer.txt"; // 학생들의 답안이 있는 텍스트 파일
	char *ssu_res = "ssu_res.txt"; // 학생들의 답안 채점 결과가 있는 텍스트 파일
	FILE *fp;
	int i, j = 0;
	Student list[STUDENT_NUM];

	for (i = 0; i < STUDENT_NUM; i++) {

		char temp[BUFFER_SIZE];
		if ((fp = fopen(ssu_answer, "r")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", ssu_answer);
			exit(1);
		}

		for (j = 0; j <= i; j++) {
			memset(temp, 0, BUFFER_SIZE);
			fgets(temp, BUFFER_SIZE, fp);
			strncpy(list[i].name, temp, strlen(temp) - 1);
			memset(temp, 0, BUFFER_SIZE);
			fgets(temp, BUFFER_SIZE, fp);
		}

		list[i].score = 100;

		for (j = 0; j < Q_SIZE; j++) {
			if (temp[j] != answer[j]) {

				list[i].score -= 10;
				list[i].res[j] = 'X';

			} else 
				list[i].res[j] = 'O';
		}
		list[i].res[j] = 0;

		printf("Student name : %s, score : %d, res : %s\n", list[i].name, list[i].score, list[i].res);
		sprintf(temp, "%s |%d| %s\n", list[i].name, list[i].score, list[i].res);
		fclose(fp);
		if ((fp = fopen(ssu_res, "a")) == NULL) {
			fprintf(stderr, "fopen error for %s\n", ssu_res);
			exit(1);
		}
		fseek(fp, 0, SEEK_END);
		fputs(temp, fp);
		fclose(fp);

	}
	fclose(fp);

	exit(0);
}
