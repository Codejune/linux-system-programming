#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person {
	char name[10];
	int age;
	double height;
} Person;

int main(void)
{
	FILE *fp;
	int i, res;
	Person ary[3] = {{"Hong GD", 500, 175.4},
		{"Lee SS", 350, 180.0},
		{"King SJ", 500, 178.6}};
	Person tmp;

	if ((fp = fopen("ftest.txt", "w")) == NULL) {
		fprintf(stderr, "fopen error for ftest.txt\n");
		exit(1);
	}

	for (i = 0; i < 3; i++) {
		strcpy(tmp.name, ary[i].name);
		tmp.age = ary[i].age;
		tmp.height = ary[i].height;
		fwrite(&tmp, sizeof(Person), 1, fp);
	}
	fclose(fp);

	if ((fp = fopen("ftest.txt", "r")) == NULL) {
		fprintf(stderr, "fopen error for ftest.txt\n");
		exit(1);
	}

	printf("[ First print]\n");
	while (!feof(fp)) {
		fread(&tmp, sizeof(Person), 1, fp);
		if(feof(fp))
			break;
		printf("%s %d %.2lf\n", tmp.name, tmp.age, tmp.height);
	}

	rewind(fp);

	printf("[ Second print]\n");
	while(!feof(fp)) {
		fread(&tmp, sizeof(Person), 1, fp);
		if(feof(fp))
			break;
		printf("%s %d %.2lf\n", tmp.name, tmp.age, tmp.height);
	}
	fclose(fp);

	exit(0);
}
