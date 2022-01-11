#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>

pid_t pid;

int rdrand_gen (uint64_t *random){
    unsigned char ok;
    __asm__ volatile ("rdrand %0; setc %1" : "=r" (*random), "=qm" (ok));
    return (int)ok;
}

static void signal_handlerSIGTERM (int signo){
    char *string = "\nSIGTERM Signal Handler of SR\n";
    write(0, string, strlen(string));
}

static void signal_handlerSIGALRM (int signo){
    char *string = "\nSIGALRM Signal Handler of SR\n";
    write(0, string, strlen(string));
    uint64_t r = 0;
    rdrand_gen(&r);
    union sigval value;
    value.sival_ptr = (void *)r;
    value.sival_int = -1;
    if(sigqueue(pid, SIGTERM, value) == 0) {
        return;
    } else {
        perror("SIGSENT-ERROR:");
    }
}

int main(int no, char *args[]){
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
    interval.it_value.tv_sec = 2.5;
    interval.it_value.tv_usec = 250000;	
    interval.it_interval = interval.it_value;
    if (setitimer(ITIMER_REAL, &interval, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    raise(14);
    while(1);
}