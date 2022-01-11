#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/socket.h>

#define reader 448
#define flag 0

int main(){
    for(int i = 0; i < 70; i++){
        unsigned long data = syscall(reader,flag);
        printf("%d) Consumer has consumed : %lu\n", i+1, data);
    }
}