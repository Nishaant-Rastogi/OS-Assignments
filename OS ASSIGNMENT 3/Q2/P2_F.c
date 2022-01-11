#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd1;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    int i = 0;
    char randomString[1000] = {0}, *indexWrite = malloc(1000);
    while (1)
    {
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, randomString, 1000);
        printf("%s", (randomString));
        close(fd1);
        int len = strlen(randomString);

        const char *index = &randomString[len-3];
        i = atoi(index);
        fd1 = open(myfifo,O_WRONLY);
        sprintf(indexWrite, "%d",i);
        write(fd1, indexWrite, strlen(indexWrite)+1);
        printf("Sent to Server: %d\n\n",i);
        if(i == 49)break;
        close(fd1);
    }
    exit(0);
}