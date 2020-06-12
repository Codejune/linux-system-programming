#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <fcntl.h> 

#define NAMESIZE 50 
#define DUMMY 0

struct employe{
	char name[NAMESIZE]; 
	int salary;
	int pid;
};

int main(int argc, char *argv[]) 
{
	struct flock lock;
	struct employe record; 
	int fd, flags, length, pid;

	if (argc < 2) {
		fprintf(stderr, "Usage :%s file \n",argv[0]);
		exit(1); 
	}

	if((fd = open(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "Open eror :%s file \n",argv[1]); 
		exit(1);
	}

	if ((flags = fcntl(fd, F_GETFL, DUMMY)) == -1) {
		fprintf(stderr, "fcntl F_GETFL error\n");
		exit(1);
	}

	flags |= O_APPEND;

	if (fcntl(fd, F_SETFL, flags) == -1) {
		fprintf(stderr, "fcntl F_SETFL error\n");
		exit(1);
	}

	pid = getpid();

	while (1) {
		printf("Enter employee name : ");
		scanf("%s", record.name);

		if(record.name[0] == '.')
			break;

		printf("Enter employee salary : ");
		scanf("%d", &record.salary);
		record.pid = pid;
		length = sizeof(record);

		if (write(fd, (char *)&record, length) != length) {
			fprintf(stderr, "record write error\n");
			exit(1);
		}
	} 
	
	close(fd); 
	exit(0);
}

