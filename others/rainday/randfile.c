#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE 1024

int main(int argc,char **argv)
{
	int fid;
	long long num_counts,i,j;
	long long sum = 0;
	int tmp,len,n;
	int t;

	int buf[BUF_SIZE];

	//printf("sizeof off_t%d\n",sizeof(off_t));

	if(argc != 3)
	{
		printf("usage: filename num_counts\n");
		return -1;
	}

	if(access(argv[1],F_OK) == 0)
	{
		printf("error ! %s is already exist\n",argv[1]);
		return -1;
	}

	sscanf(argv[2],"%lld",&num_counts);
	if( num_counts < 0 && num_counts >= 900000000)
	{
		printf("error num_counts!\n");
		return -1;
	}
//S_IRUSR|S_IWUSR
	umask(0);
	if((fid=creat(argv[1],0666)) == 0)
	{
		printf("error to creat %s\n",argv[1]);
		return -1;
	}
     srand( (int)time(0) ); 
	if(num_counts <= BUF_SIZE)
	{
		for( i = 0; i < num_counts; ++i)
		{
			tmp = (rand() + 1)%100000;//1000;
			sum += tmp;

			if(write(fid,&tmp,sizeof(int)) < 0)
			{
				printf("write error\n");
				goto ERR_RETURN;
			}
		}
	}
	else
	{
		n = num_counts/BUF_SIZE;
		for( i = 0; i < n; ++i)
		{
			for(j = 0; j < BUF_SIZE; ++j)
			{
				tmp = (rand()+1)%100000;	
				sum += tmp;
				buf[j] = tmp;
			}
			if(write(fid,&buf,BUF_SIZE) < 0)
			{
				printf("write error\n");
				goto ERR_RETURN;
			}

		}

		for( i = 0; i < num_counts - n*BUF_SIZE;++i)
		{
			tmp = (rand() + 1)%100000;//1000;
			sum += tmp;

			if(write(fid,&tmp,sizeof(int)) < 0)
			{
				printf("write error\n");
				goto ERR_RETURN;
			}

		}
	}
	

ERR_RETURN:
	close(fid);
	printf("sum = %lld\n",sum);
	return 0;
}
