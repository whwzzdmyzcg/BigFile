#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<sys/wait.h>

int main(int argc,char * argv[])
{

	int i,status;      
	pid_t pid[4],j;		
	
	for (i=1;i<=4;i++)
	{
		if((pid[i-1] =fork()) < 0)
		{
			printf("fork1 error\n");
		}
		else if(pid[i-1] == 0)
		{
			execlp("./count","count",argv[i],(char *)NULL);   //执行完后不会再返回执行后面的了
		}
	}
	for (j=1;j<=4;j++)
		waitpid(pid[j-1],NULL,0);
	exit(1);
}
