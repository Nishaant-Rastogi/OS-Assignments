#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

static char *randomString(char *str){
    size_t size = 20;
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
    }
    return str;
}
int main()
{
    int fd;
    char *randomStrings[50];
    for(int i = 0; i<50; i++){
        char *randStr = malloc(20);
        randomStrings[i] = randomString(randStr);
        if(i <= 9){
            sprintf(randomStrings[i],"%s 0%d\n",randomStrings[i],i);
        }else{ 
            sprintf(randomStrings[i],"%s %d\n",randomStrings[i],i);
        }
    }
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char indexRead[1000],random[10000];
    int ID = 0;
    while (ID < 50)
    {
        for(int i = ID; i<ID+5;i++){
            strcat(random,randomStrings[i]);
        }
        fd = open(myfifo, O_WRONLY);
        write(fd, (random), strlen(random));
        close(fd);
        random[0] = '\0';
        fd = open(myfifo, O_RDONLY);
        read(fd, indexRead, sizeof(indexRead));
        ID = atoi(indexRead)+1;
        printf("Sent by Client: %s\n", indexRead);
        close(fd);
    }
    exit(0);
}