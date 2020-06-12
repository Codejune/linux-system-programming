#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

char *buf;
int readsize;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *read_thread(void *arg)
{
	int num;
	int fd;

	while(readsize < sizeof(buf)) {
		pthread_mutex_lock(&mutex);

		if ((fd = open("test.txt", O_RDONLY)) < 0) {
			fprintf(stderr, "open error for test.txt\n");
			return NULL;
		}

		lseek(fd, readsize, SEEK_SET);

		read(fd, buf + readsize, 1);
		readsize++;
		close(fd);

		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(void)
{
	struct stat stat_buf;
	int status;
	int thread_num;
	pthread_t tid;

	printf("Number of thread : ");
	scanf("%d", &thread_num);

	lstat("test.txt", &stat_buf);
	buf = (char *)calloc(stat_buf.st_size + 1, sizeof(char));
	readsize = 0;

	for (int i = 0; i < thread_num; i++)
		pthread_create(&tid, NULL, read_thread, NULL);

	while (readsize < sizeof(buf))
		sleep(1);

	pthread_mutex_destroy(&mutex);

	buf[readsize] = 0;
	printf("%s\n", buf);
	exit(0);
}
