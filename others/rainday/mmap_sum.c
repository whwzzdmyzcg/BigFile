#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_BUF (4096*4)
typedef long long value_type;

typedef struct COMPUTE_INFO
{
	off_t pos;
	off_t nbytes;
}COMPUTE_INFO,*PCOMPUTE_INFO;

value_type mmap_compute(off_t pos,off_t nbytes);
void thread_compute(void *param);

char *g_src = NULL;
int g_nthreads = 0;
value_type g_final_res = 0;
pthread_mutex_t nthreads_mutex;

int main(int argc,char **argv)
{
	int fid;
	off_t size,len,last_len;
	off_t off;
	int thread_counts = 0;
	int i = 0;

	struct stat statbuf;
	pthread_t thread_id;
	pthread_attr_t thread_attr;
	PCOMPUTE_INFO pcompute_info = NULL;

	clock_t start_time,end_time;
	double total_time;
	
	start_time = clock();

	if(argc != 2)
	{
		printf("usage: filename\n");
		return -1;
	}
	
	if( (fid = open(argv[1],O_RDONLY))< 0 )
	{
		printf("error to open file %s\n",argv[1]);
		return -1;
	}

	if( fstat(fid,&statbuf) < 0 )
	{
		printf("fstat error\n");
		return -1;
	}
	
	if( (g_src = mmap(0,statbuf.st_size,PROT_READ,MAP_PRIVATE,fid,0)) == (char *)-1 )
	{
		printf("error to map\n");
		return -1;
	}

	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&nthreads_mutex,NULL);

	g_final_res = 0;
	g_nthreads = 0;


	len = 4000000;
	if( statbuf.st_size<= len)
		thread_counts = 1;
	else
	{
		thread_counts = statbuf.st_size / len;
	}
	
	thread_counts = 128;
	len = statbuf.st_size / thread_counts;

	pcompute_info = malloc(sizeof(COMPUTE_INFO)*(thread_counts+2));

	for(i = 0; i < thread_counts; ++i)
	{
		pcompute_info[i].pos = i*len ;
		pcompute_info[i].nbytes = len;

//		g_final_res += mmap_compute(i*len,len);
		if((thread_id = pthread_create(&thread_id,&thread_attr,(void *)thread_compute,(void *)&pcompute_info[i])) != 0)
		{
			printf("pthread_create error\n");
			exit(0);
		}
	}

	if( statbuf.st_size > len * thread_counts )
	{
		last_len = statbuf.st_size - len * thread_counts;
		++thread_counts;

		pcompute_info[i].pos = (thread_counts -1)*len ;
		pcompute_info[i].nbytes = last_len;
	//	g_final_res += mmap_compute((thread_counts-1)*len,last_len);
		if(pthread_create(&thread_id,&thread_attr,(void *)thread_compute,(void *)&pcompute_info[i]) != 0)
		{
			printf("pthread_create error\n");
			exit(0);
		}
	}

//	printf("thread counts:%d\n",thread_counts);
	
	while(g_nthreads < thread_counts)
		;

	pthread_mutex_destroy(&nthreads_mutex);
	free(pcompute_info);
//	len = statbuf.st_size;
//	g_final_res = mmap_compute(0,len);

	end_time = clock();
	total_time = (double)((end_time- start_time)/(double)CLOCKS_PER_SEC);

//	printf("time use:%f\n",total_time);
	printf("%s  %lld\n",argv[1],g_final_res);
//	printf("MAX_BUF %d\n",MAX_BUF);
	return 0;
}
void thread_compute(void *param)
{
	PCOMPUTE_INFO pinfo = (PCOMPUTE_INFO)param;
	value_type res = 0;
	res = mmap_compute(pinfo->pos,pinfo->nbytes);

	pthread_mutex_lock(&nthreads_mutex);
	g_final_res += res;
	++g_nthreads;
//	printf("thread res=%lld\n",res);
	pthread_mutex_unlock(&nthreads_mutex);

	pthread_exit(NULL);
}
value_type mmap_compute(off_t pos,off_t nbytes)
{
	off_t max_bytes = MAX_BUF * 4;
	int buf[MAX_BUF];
//	int *buf = NULL;

	off_t nlast = nbytes; //剩余要读取的字节数
	off_t ntmp = 0;//每一次要读取的字节数
	off_t num_count = 0;

	int i,k,tmp;
	value_type sum = 0;

//	printf("pos = %d\n",pos);
//	printf("len = %d\n",nbytes);

	sum = 0;
	while(1) 
	{
		ntmp = nlast < max_bytes? nlast : max_bytes;

		memcpy(buf,g_src+pos,ntmp);
	//	buf = g_src + pos;
		pos += ntmp;
		nlast -= ntmp;

		num_count = ntmp/4;

		for( i = 0; i < num_count; ++i)
			sum += buf[i];

		if(nlast <=0 )
			break;
	}

	return sum;
}
