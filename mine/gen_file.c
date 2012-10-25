/*
 * =====================================================================================
 *
 *       Filename:  gen_file.c
 *
 *    Description:  生成随机数文件
 *
 *        Version:  1.0
 *        Created:  10/25/2012 04:34:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h> // open
#include <stdlib.h> // rand,srand
#include <errno.h> // perror

#define	LENGTH 100000000

int	buf[LENGTH] = {0};

int main(int argc, char *argv[])
{
	int	i,j,n;
	int	fd;
	// 检查命令行参数
	if (argc < 2)
	{		
		printf("Usage:./%s File0[ File1 File2 ...]\n", argv[0]);
		perror(argv[0]);
		return -1;
	}

	// 生成随机数并写入文件File0[ File1 Fil2 ...]
	for(j = 1; j < argc; j++)
	{
  		if ((fd = open(argv[j], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR)) == -1)
   		{
			perror(argv[j]);
			return -1;
		}

	  	srand((int)time(0));

  		for (i = 0; i < LENGTH; i++)
	  	{
    			buf[i] = 1+(int)(10.0*rand()/(RAND_MAX+1.0));	
		}

		if (write(fd, buf, LENGTH*4) == -1) // 此处长度参数的单位为字节！！！
		{		
			perror(argv[j]);
			return -1;
		}
	}

	return 0;
}
