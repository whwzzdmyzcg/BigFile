#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define MAX_BUF 1024

int main(int argc,char **argv)
{
	int fid = -1;
	int t = 0;
	int n;
	char buf[MAX_BUF];

	if( (fid = open(argv[1],O_RDONLY)) < 0)
	{
		printf("failed to open file: %s\n",argv[1]);
		exit(1);
	}

	while( (n=read(fid,&t,sizeof(int)) ) > 0)
	{
		printf("num=%d:bit=%d\n",t,n);
	}
	
	close(fid);
	return 0;
}
