#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

/* 如果对于每个文件调用函数，不能实现并行处理。有两种简单的思路：
 * 1 对于每个文件使用一个子进程exec指定程序进行处理，读取内容求和,该程序本身使用多线程分段处理
 * 主程序控制父 子进程的调用顺序，最后输出最终结果
 * 2 主进程本身使用多线程打开各个文件进行处理。
 * 待解决问题：
 * 僵死进程的处理 利用信号处理
 */

void func_show_help();
void func_sig_chld(int signum);

//计算一个文件的所有数字之和
long long func_sum(char *file);
int g_process_count = 0;

int main(int argc, char *argv[])
{
	int i = 0;
	pid_t pid = 0;
	g_process_count = 0;
	char *sum_prc = "io_sum";

	if(argc < 2)
		func_show_help();
/*
	if(signal(SIGCHLD,func_sig_chld) == SIG_ERR)
	{
		printf("sig error\n");
		return -1;
	}
*/
	for( i = 1 ; i < argc; ++i)
	{
		if((pid = fork()) < 0) //every new process for every file
		{
			printf("fork error: %d",i);
		}
		else if(pid == 0)
		{
			if(execl(sum_prc,sum_prc,argv[i],(char*)0) < 0)
				printf("exec error\n");
			return 0;
		}
	}

for( i = 0; i < argc -1 ; ++i)
	wait(NULL);

//	while(g_process_count < argc-1);
//	printf("g_process_count = %d\n",g_process_count);

	return 0;
}

void func_show_help()
{
	printf("usage: main1 filename1 filename2 ...\n");
	exit(0);
}
void func_sig_chld(int signum)
{
/*
	sigset_t newmask,oldmask,waitmask;
	sigemptyset(&newmask);	
	sigemptyset(&waitmask);	
	sigaddset(&newmask,SIGCHLD);

	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask))
	{
		printf("sigprocmaks error!\n");
		exit(0);
	}

	if(sigsuspend(&waitmask) != -1)
	{
       printf("sigsuspend error\n");
		exit(0);
	}
*/
	int pid;
	pid = wait(NULL);
	++g_process_count;
	printf("sig_child process_count = %d\n",g_process_count);
/*	
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL))
	{
		printf("sigprocmaks error!\n");
		exit(0);
	}
*/
}
