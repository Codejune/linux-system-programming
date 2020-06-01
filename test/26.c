#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char character;
	char buf[BUFFER_SIZE];


	// getchar(), putchar() 사용
	while((character = getchar()) != EOF)
		putchar(character);

	if (ferror(stdin)) {
		fprintf(stderr, "standard input error\n");
		exit(1);
	}

	// getc(), putc() 사용
	while((character = getc(stdin)) != EOF)
		putc(character, stdout);
	
	if (ferror(stdin)) {
		fprintf(stderr, "standard input error\n");
		exit(1);
	}

	// fgets(), fputs() 사용
	fgets(buf, BUFFER_SIZE, stdin);
	fputs(buf, stdout);
	
	if (ferror(stdin)) {
		fprintf(stderr, "standard input error\n");
		exit(1);
	}
	memset(buf, 0, sizeof(buf));

	// scanf(), printf() 사용
	scanf("%s", buf);
	printf("%s\n", buf);
	
	exit(0);
}

