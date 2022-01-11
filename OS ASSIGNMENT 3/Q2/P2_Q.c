#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct messageBuffer {
    long mesgType;
    char mesgText[1000];
} ;
  
int main()
{
    key_t key1,key2;
    int msgid1,msgid2;
    struct messageBuffer *message1 = malloc(sizeof(struct messageBuffer));
    struct messageBuffer *message2 = malloc(sizeof(struct messageBuffer));
    
    int ID=0;

    while(ID<49){
        sleep(0.6);
        key1 = ftok("progfile1", 65);
        msgid1 = msgget(key1, 0666 | IPC_CREAT);
        
        msgrcv(msgid1, message1, sizeof(struct messageBuffer), 1, 0);

        printf("%s", message1->mesgText);
        msgctl(msgid1, IPC_RMID, NULL);
        
        key2 = ftok("progfile2",75);
        msgid2 = msgget(key2, 0666 | IPC_CREAT);
        message2->mesgType = 2;
        int len = strlen(message1->mesgText);
        const char *index = &message1->mesgText[len-3];
        ID = atoi(index);
        sprintf(message2->mesgText,"%d",ID);
        if(msgsnd(msgid2, message2, sizeof(struct messageBuffer), 0)!=0){
            perror("msgsnd");
        }
        printf("Sent to Server: %s\n\n",message2->mesgText);
        message2->mesgText[0] = '\0';
    }
    return 0;
}