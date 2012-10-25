/*
 * =====================================================================================
 *
 *       Filename:  parent.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2012 04:40:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */

#include <unistd.h> // fork
#include <sys/types.h> // fork,waitpid
#include <sys/wait.h> // waitpid
#include <stdio.h> // fprintf
#include <stdlib.h> // exit
#include <errno.h> // perror

int main(int argc, char *argv[])  
{ 
	int	i;
	pid_t	pid[argc-1];

	for (i = 1; i < argc; i++) // 创建argc-1个子进程
	{
		// 创建进程失败
		if ((pid[i-1] = fork()) < 0)
		{
			perror(argv[0]);
			return -1;
		}
		// 子进程
      	else if (0 == pid[i-1])
		{
			// 调用可执行文件child
			// child:路径
			// child, argv[i]:参数
			if (execl("child-1", "child-1", argv[i], (char *)0) < 0)	            
			{
				perror(argv[0]);
				return -1;
			}
		}
	}

	// 父进程
	for (i = 0; i < argc-1; i++)
		if (waitpid(-1, NULL, 0) < 0)
		{
			perror(argv[0]);
			return -1;
		}

	return 0;
} 
