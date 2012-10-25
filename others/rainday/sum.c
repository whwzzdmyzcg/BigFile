
#define _FILE_OFFSET_BITS 64
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#define MAXBUF 1024

int main(int argc, char *argv[])
{
	int file_id;
	char buf[MAXBUF];
	long long sum = 0;
	int n,i,tmp = 0;
	char *head = NULL; 
	char *tail = NULL; 

	int is_end = -1;

	clock_t start_time,end_time;
	double total_time;

	start_time = clock();

	if( (file_id = open(argv[1],O_RDONLY)) < 0)
	{
		printf("failed to open file: %s\n",argv[1]);
		exit(1);
	}
	if(  (n = read(file_id,buf,MAXBUF)) > 0)
	{
		buf[n] = '0';
		printf("buf=%s\n",buf);
	}
	else
	{
		printf("read error\n");
	}
/*
	while( (n = read(file_id,buf,MAXBUF)) > 0 )
	{
		//printf("n=%d\n",n);	
	//	printf("buf[0]=%c\n",buf[0]);
	//	printf("buf[n-1]=%c\n",buf[n-1]);

		head = buf;
		tail = buf;

		i = 0;
		for(i = 0; i < n; ++i,++tail)
		{
			if( i == n-1 && n < MAXBUF )
				is_end = 1;
			else
				is_end = 0;

			if(*tail == ' ' || is_end) //文件尾或者空格
			{
				*tail = '\0';
				sscanf(head,"%d",&tmp);
			//	printf("num=%s\n",head);
				head = tail + 1;

				sum += tmp;
			}
		}

		if( n == MAXBUF && buf[n-1] != ' ') //如果一个数字读取时被截断，从head处重新开始读取
		{
			lseek(file_id, head - buf - n,SEEK_CUR);
		}
		
	}
*/
	end_time = clock();
	total_time = (double)((end_time- start_time)/(double)CLOCKS_PER_SEC);

	printf("time use:%f\n",total_time);
	printf("%s = %lld\n",argv[1],sum);

	return 0;
}
