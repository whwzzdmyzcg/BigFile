/*
 * =====================================================================================
 *
 *       Filename:  child.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2012 04:43:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sys/types.h> // open,fstat
#include <sys/stat.h> // open,fstat
#include <fcntl.h> // open
#include <unistd.h> // mmap,munmap
#include<sys/mman.h> // mmap,munmap
#include <stdio.h> // fprintf
#include <pthread.h> // pthread_create,pthread_join
#include <stdlib.h> // exit
#include <errno.h> // perror,strerror

#define LENGTH 100000000
#define PART 4 // 线程数
#define UNITSIZE 4096
#define ALPTH (long)(400000000/PART/4096+1)

long long sum[PART] = {0};
int fd;

void *calc(void *arg)
{
	volatile int i;
	volatile int j = (int)arg; // !!!
	int *start;
	struct stat sb;

	fstat(fd, &sb); // 取得文件大小

	// 最后一部分
	if (j == PART-1)
	{
		// mmap及munmap-http://man.chinaunix.net/develop/c&c++/linux_c/default.htm
		if ((start = mmap(NULL, LENGTH*4-ALPTH*UNITSIZE*(PART-1), PROT_READ, MAP_PRIVATE, fd, j*ALPTH*UNITSIZE)) == MAP_FAILED)
		{
			fprintf(stderr, "ERROR: %d\n", strerror(errno));
			return -1;
		}
		for (i = 0; i < (LENGTH*4-ALPTH*UNITSIZE*(PART-1))/4; i++)
			sum[j] += *(start+i);
		munmap(start,LENGTH*4-ALPTH*UNITSIZE*(PART-1));  // *解除映射
	}
	// 前面的PART-1个部分
	else
	{
		if ((start = mmap(NULL, ALPTH*UNITSIZE, PROT_READ, MAP_PRIVATE, fd, j*ALPTH*UNITSIZE)) == MAP_FAILED)
		{
			fprintf(stderr, "ERROR: %d\n", strerror(errno));
			exit(1);
		}
		for (i = 0; i < ALPTH*UNITSIZE/4; i++)
			sum[j] += *(start+i);
		munmap(start,ALPTH*UNITSIZE); //*解除映射
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int j;
	long long sum_all = 0;
	
	pthread_t calc_id[PART];
	void *status;
		
	if (-1 == (fd = open(argv[1], O_RDONLY)))
	{
		perror(argv[0]);
		exit(1);
	}

	for (j = 0; j < PART; j++)
	{
		// 创建线程，线程执行calc
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
