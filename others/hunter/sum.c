#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

#define max 100000000
int buf[max];

int main(int argc,char *argv[])
{
int fd,a,n;

/*int no;
char no1;*/

long long  sum=0;

/*no1=*argv[2];*/

fd=open(argv[1],O_RDWR);
if(fd==-1)
printf("open error\n");

a=read(fd,buf,max*4);
if(a==-1)
printf("read error\n");

for(n=0;n<max;n++)
sum=sum+buf[n];


/*no=(int)no1;
printf("%d\n",no);*/

printf("the sum is:%lld\n",sum);

}

