#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
   
#define ADDRESS     "Socket"  

int main()
{
    char c;
    FILE *fp;
    register int i, s, len;
    struct sockaddr_un saun;
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("client: socket");
        exit(1);
    }
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, ADDRESS);
    len = sizeof(saun.sun_family) + strlen(saun.sun_path);

    if (connect(s, (struct sockaddr *)&saun, len) < 0) {
        perror("client: connect");
        exit(1);
    }
    fp = fdopen(s, "r");
    int ID = 0;
    while(1){
        char randomString[1000];
        int k = 0;
        for (i = 0; i < 5; i++) {
            while ((c = fgetc(fp)) != EOF) {
                putchar(c);
                if (c == '\n')
                    break;
                randomString[k] = c;
                k++;
            }
        }
        ID = (randomString[k-2] -'0')*10  + (randomString[k-1] -'0');
        char indexWrite[100];
        sprintf(indexWrite, "%d", ID);
        printf("Sent to Server: %s\n\n",indexWrite);
        send(s, indexWrite, 2, 0);
        if(ID == 49) break;
    }
    close(s);
    exit(0);
}