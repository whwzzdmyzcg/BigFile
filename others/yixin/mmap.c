#include<sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF  4194304    //4*1024*1024
#define LEN      16777216  //16*1024*1024 = 16M
#define NUM      24      //线程数

int* buf;
double total_all = 0;
double total[24];
struct args
{
	int i,fd;
	int *mm;
};

void* thread_count(struct args* arg)
{         
	int rd;                  //读的字节数
	int j = 0;
	extern int errno;

	if(arg->i != NUM-1)
	{	
		if((arg->mm = mmap(NULL,LEN,PROT_READ,MAP_SHARED,arg->fd,(arg->i)*LEN)) == MAP_FAILED) 
			printf("mmap error~~\n");
		else
		{		
			while(j!= MAX_BUF)
			{	
				total[arg->i] = total[arg->i] + arg->mm[j];		
				j++;
			}          
		} 
	}
	else  
	{
		lseek(arg->fd,(arg->i)*LEN,SEEK_SET);		
		buf = (int*)malloc(sizeof(int)*MAX_BUF);
		if((rd = read(arg->fd,buf,LEN)) == -1)        //读文件
			printf("Read error\n");
		else
		{
			rd = rd/4;
			while(j != rd)
			{
				total[arg->i] = total[arg->i] + buf[j];
				j++;
			}                
		} 
	}
	//printf("total[%d] = %.0f\n",arg->i,total[arg->i]);		
	total_all += total[arg->i];
}

int main(int argc,char* argv[])                //第一个参数为输入文件名
{
	pthread_t thread_id[NUM];		
	int ret[NUM],j;
	struct args *arg;
	arg = (struct args*)malloc(sizeof(struct args*));

	if((arg->fd = open(argv[1],O_RDONLY,S_IRUSR))==-1)         
	{
		fprintf(stderr,"Open %s Error:%d\n",argv[1],strerror(errno));
		exit(1);
	}
	else 
	{
		for ((arg->i) = 0;(arg->i)<NUM;(arg->i)++)
		{
			total[arg->i] = 0;
			ret[arg->i] = pthread_create(&thread_id[arg->i],NULL,(void*)thread_count,(struct args*)arg); 
			pthread_join(thread_id[arg->i],NULL);
		}
		//for (j = 0;j<NUM;j++)
		//	pthread_join(thread_id[j],NULL);
	}
	printf("%s:%.0f\n",argv[1],total_all);
	return(1);
}


