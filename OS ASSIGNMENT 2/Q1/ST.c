#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pid_t pid;

static unsigned long long rdtsc(void){
    unsigned l, r;
    __asm__ volatile ("rdtsc" : "=a"(l), "=d"(r));
    return ( (unsigned long long)l)|( ((unsigned long long)r)<<32 );
}

static void signal_handlerSIGTERM (int signo){   
    char *string = "\nSIGTERM Signal Handler of ST\n";
    write(0, string, strlen(string));
}

static void signal_handlerSIGALRM (int signo){   
    char *string = "\nSIGALRM Signal Handler of ST\n";
    write(0, string, strlen(string));
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    char *timeString = (char*) shmat(shmid,(void*)0,0);
    
    uint64_t timeStamp = rdtsc();
    uint64_t timeInSeconds = (timeStamp/(2.60*1000000000));
    uint64_t hours = timeInSeconds/3600;
    uint64_t minutes = timeInSeconds%3600/60;
    uint64_t seconds =  timeInSeconds % 3600 % 60;
    snprintf(timeString,1000,"CPU Timestamp String: %llu:%llu:%llu\n", hours,minutes,seconds);
    union sigval value;
    value.sival_int = timeInSeconds;
    if(sigqueue(pid, SIGTERM, value) == 0) {
        return;
    } else {
        perror("SIGSENT-ERROR:");
    }
    shmdt(timeString);
}

int main(int no,char *args[]){
    pid = atoi(args[0]);
    if (signal (SIGALRM, signal_handlerSIGALRM) == SIG_ERR) {
        perror("SIGALRM");
        exit (-1);
    }
    if (signal (SIGTERM, signal_handlerSIGTERM) == SIG_ERR) {
        perror("SIGTERM");
        exit (-1);
    }
    struct itimerval interval;
    interval.it_value.tv_sec = 2;
    interval.it_value.tv_usec = 200000;	
    interval.it_interval = interval.it_value;
    if (setitimer(ITIMER_REAL, &interval, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    raise(14);
    while(1);
}