#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

static void signal_handlerSIGTERM (int signo , siginfo_t *info, void *context){   
    char *string = "SIGTERM Signal Handler of S1\n";
    write(0, string, strlen(string));
    if(info->si_int > 0){
        key_t key = ftok("shmfile",65);
        int shmid = shmget(key,1024,0666|IPC_CREAT);
        char *timeString = (char*) shmat(shmid,(void*)0,0);
        write(0,timeString,strlen(timeString));
        shmdt(timeString);
    }
    else{
        char randomString[1000];
        snprintf(randomString, 1000, "Random Number: %llu\n",info->si_ptr);
        write(0,randomString,strlen(randomString));
    }
}

int main(){
    pid_t pId;
    pid_t SR;
    pid_t ST;
    int status;
    pId = fork();
    if(pId==0){
        //S1
        int pid =  getpid();
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = signal_handlerSIGTERM;
        sigaction(SIGTERM, &sa,0);
        while(1){};
    }
    else if(pId>0){
        char *args = malloc(13);
        sprintf(args, "%d", pId);
        SR = fork();
        if(SR==0){
            //SR
            execl("./E1",args,NULL);
        }
        else if(SR>0){
            int status;
            ST = fork();
            if(ST==0){
                //ST
                execl("./E2",args,NULL);
            }
            else if(ST<0){
                perror("fork");
            }
        }
        else{
            perror("fork");
        }
    }
    if(pId<0){
        perror("fork");
    }
    waitpid(pId,&status,0);
    waitpid(SR,&status,0);
    waitpid(ST,&status,0);  
    exit(0);
}