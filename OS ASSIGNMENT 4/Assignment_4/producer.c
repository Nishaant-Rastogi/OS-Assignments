#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/socket.h>

#define writer 449
#define size 8

unsigned long int produce(unsigned long int data, unsigned char buf[]){
    for(int j = 0; j < size; j++){
        data <<= size;
        data |= buf[j];
    }
    return data;
}
int main(){
    for(int i = 0; i < 70; i++){
        int fd = open("/dev/urandom", O_RDONLY);
        unsigned long int data = 0;
        unsigned char buf[size];
        read(fd, buf, size);
        data = produce(data, buf);
        syscall(writer, data);
        printf("%d) Producer has produced : %lu\n", i+1, data);
    }
}