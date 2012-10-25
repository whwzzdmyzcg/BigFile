#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define MAX_BUF (4096*4)
typedef long long value_type;

typedef struct COMPUTE_INFO
{
	int fd;
	off_t pos;
	size_t nbytes;
}COMPUTE_INFO,*PCOMPUTE_INFO;

//计算某文件一段长度的大小
//fd:文件描述符
//pos：文件起始位置
//len：需要计算的长度
//注意：为了提高效率，未进行pos,nbytes的合法检查
//利用缓冲读取的时候，某次可能会截断一个数据，利用lseek的回退进行处理，即下次从这个数据的开始处重新读。
//但如果下次要读的剩余数据总量和当前数据被截断的部分相同，而当前数据下一位恰好是空格，应作特殊处理。

value_type compute(int fd,off_t pos,off_t nbytes);
void thread_compute(void *param);

int g_nthreads = 0;
pthread_mutex_t nthreads_mutex;

value_type g_final_res = 0;
int main(int argc,char **argv)
{
	int file_id = 0;
	int tmp_fd = 0;
	pid_t pid = 0;

	int i = 0;
	value_type len = 1000; //the length of the file every process to compute

	int n_process = 1;
	struct stat stat_buf;
	off_t file_size = 0;

	pthread_t thread_id;
	pthread_attr_t thread_attr;

	PCOMPUTE_INFO pcompute_info = NULL;
	clock_t start_time,end_time;
	double total_time;
	
	start_time = clock();


	if(argc != 2)
	{
		printf("error to use sumfid\n");
		return -1;
	}
	if( (file_id = open(argv[1],O_RDONLY)) < 0)
	{
		printf("error to open file %s:\n",argv[1]);
		exit(1);
	}
	if(fstat(file_id,&stat_buf) < 0)
	{
		printf("error to get file stat\n");
		return -1;
	}

	if( (len*n_process < stat_buf.st_size)) //once the file is not large enough
	{
		n_process = stat_buf.st_size / len;
	}
	

	if(n_process * len < stat_buf.st_size)
	{
		++n_process;
	}
	
	n_process = 128;
	len = stat_buf.st_size / n_process; 

	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
	pthread_mutex_init(&nthreads_mutex,NULL);

	g_final_res = 0;

	pcompute_info = malloc(sizeof(COMPUTE_INFO)*(n_process+1));

	for(i = 0; i < n_process; ++i)
	{
		//g_final_res += compute(open(argv[1],O_RDONLY),i*len,len);

		pcompute_info[i].fd = open(argv[1],O_RDONLY);
		pcompute_info[i].pos = i*len ;
		pcompute_info[i].nbytes = len;
		if(pthread_create(&thread_id,&thread_attr,(void *)thread_compute,(void *)&pcompute_info[i]) != 0)
		{
			printf("pthread_create error\n");
			exit(0);
		}

	}
	while(g_nthreads < n_process)
		;

	pthread_mutex_destroy(&nthreads_mutex);
	free(pcompute_info);

	end_time = clock();
	total_time = (double)((end_time- start_time)/(double)CLOCKS_PER_SEC);

//	printf("MAX_BUF %d time use:%f\n",MAX_BUF,total_time);
//	printf("thread_count = %d\n",n_process);
	printf("%s = %lld\n",argv[1],g_final_res);
	return 0;
}
value_type compute(int fd,off_t pos,off_t nbytes)
{
	int buf[MAX_BUF];
	off_t nread = 0;	//已经读取的字节数
	off_t ntmp = 0;	//每次要读取的字节数
	off_t nlast = nbytes;	//剩余未读的字节数
	off_t num_counts = 0;

	int n = 0;
	int k = 0;
	int i = 0;
	value_type sum = 0;
	
	lseek(fd,pos,SEEK_SET);

	while( (n = read(fd,buf,MAX_BUF)) > 0)
	{
		ntmp = nlast < MAX_BUF ? nlast : MAX_BUF;
		num_counts = ntmp / 4;
		nlast -= ntmp;

		for(i = 0; i < num_counts; ++i)
			sum += buf[i];

		if( nlast <=0 )
			break; 
		
	}//end of while 

	return sum;
}

void thread_compute(void *param)
{
	PCOMPUTE_INFO pinfo = (PCOMPUTE_INFO)param;
	value_type res = 0;
	res = compute(pinfo->fd,pinfo->pos,pinfo->nbytes);

	pthread_mutex_lock(&nthreads_mutex);
	g_final_res += res;
	++g_nthreads;
//	printf("thread res=%lld\n",res);
	pthread_mutex_unlock(&nthreads_mutex);
	pthread_exit(NULL);
}

