#include <sys/types.h> // open,lseek
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <unistd.h> // lseek
#include <stdio.h> // fprintf
#include <pthread.h> // pthread_create,pthread_join
#include <stdlib.h> // calloc,mallloc,free,exit
#include <errno.h> // perror,strerror

#define LENGTH 100000000
#define PART 4 // 线程数
#define UNITSIZE LENGTH/PART

long long sum[PART] = {0};
int fd;

void *calc(void *arg)
{
	volatile int i;
	volatile int j = (int)arg; // !!!
	volatile off_t curpos;

	volatile int *buf;
	buf = (volatile int*)malloc(sizeof(int)*UNITSIZE);

	if ((curpos=lseek(fd, j*UNITSIZE*4, SEEK_SET)) == -1) // 单位:字节！！！
	{
		fprintf(stderr, "ERROR: %d\n", strerror(errno));
		exit(1);
	}
	if (read(fd, (void *)buf, UNITSIZE*4) == -1)
	{
		fprintf(stderr, "ERROR: %d\n", strerror(errno));
		exit(1);
	}
	for (i = 0; i < UNITSIZE; i++)	
		sum[j] += *(buf+i);
	free((void *)buf);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int j;
	long long sum_all = 0;
	
	pthread_t	calc_id[PART];
	void 		*status;
		
	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror(argv[0]);
		exit(1);
	}
	for (j = 0; j < PART; j++)
	{
		if (pthread_create(&calc_id[j], NULL, calc, (void *)j) != 0)
		{
			perror(argv[0]);
			exit(1);
		}
	}
	for (j = 0; j < PART; j++)
		pthread_join(calc_id[j],&status);
	for (j = 0; j < PART; j++)
		sum_all += sum[j];
	fprintf(stdout,"线程数：%d\n", PART);
	fprintf(stdout,"file: %s-----sum = %lld\n", argv[1], sum_all);
	pthread_exit(status);

	return 0;
}
