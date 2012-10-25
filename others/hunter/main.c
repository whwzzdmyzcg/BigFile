#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>


int main(int argc,char *argv[]){
    int i;
    /*char *p;
      char m;*/
    pid_t pid[4];
    float start,end;
    clock_t time;
    start=clock();

    for(i=0;i<4;i++)
    {   pid[i]=fork();
        if(pid[i]<0)
            printf("fork[%d] error\n",i+1);

        if(pid[i]==0)



            /* m=(char)i;
               p=&m;*/

            execl("sum","sum",argv[i+1],(char*)0);
    }

    while(-1!=wait())
        wait();

    sleep(1);

    end=clock();
    time=(end-start)/CLOCKS_PER_SEC;

    printf("start equals %f\n",start);
    printf("end equals %f\n",end);

    printf("the time was:%f secomdes\n",time);
    return 0;
}



