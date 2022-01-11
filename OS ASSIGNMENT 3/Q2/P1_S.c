#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define ADDRESS     "Socket"  

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
    FILE *fp;
    int fromlen;
    register int socketFd, newSocket, len;
    struct sockaddr_un saun, fsaun;

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
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("Server: socket");
        exit(1);
    }
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, ADDRESS);
    unlink(ADDRESS);
    len = sizeof(saun.sun_family) + strlen(saun.sun_path);

    if (bind(socketFd,(struct sockaddr *)&saun, len) < 0) {
        perror("Server: bind");
        exit(1);
    }
    if (listen(socketFd, 3) < 0) {
        perror("Server: listen");
        exit(1);
    }
    if ((newSocket = accept(socketFd, (struct sockaddr *)&fsaun, &fromlen)) < 0) {
        perror("Server: accept");
        exit(1);
    }
    fp = fdopen(newSocket, "r");
    int ID=0;
    while(ID<50){
        char random[100];
        for (int i = ID; i < ID+5; i++)
            strcat(random,randomStrings[i]); 
        send(newSocket, random, strlen(random), 0);
        random[0] = '\0';
        char index[100];
        int valread = read(newSocket,index, 100);
        if(strlen(index) < 2)printf("Sent by Client: %s\n",index);
        else printf("Sent by Client: %s",index);
        ID = atoi(index)+1;
    }
    close(socketFd);
    exit(0);
}