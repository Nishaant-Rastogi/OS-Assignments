#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sched.h>

int main(){
    pid_t pid = fork();
    int stat;
    if(pid==0){
        double start = omp_get_wtime();
        for(long i=0; i<1000000000; i++);
        double time = omp_get_wtime() - start;
        printf("Time without Syscall: %lf \n",time);
    }
    else{
        waitpid(pid, &stat, 0);
        int fd = open("process.txt", O_RDONLY);
        int id = 0;
        int pid = read(fd, &id, sizeof(pid_t));
        syscall(448, id, 10000000);
        double start = omp_get_wtime();
        for(long i=0; i<1000000000; i++);
        double time = omp_get_wtime() - start;
        printf("Time with Syscall: %lf \n",time);
    }
}