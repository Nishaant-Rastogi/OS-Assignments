#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

struct messageBuffer {
    long mesgType;
    char mesgText[1000];
} ;
  
int main()
{   
    struct messageBuffer *message1 = malloc(sizeof(struct messageBuffer));
    struct messageBuffer *message2 = malloc(sizeof(struct messageBuffer));
    key_t key1,key2;
    int msgid1,msgid2;
    char *randomStrings[50];
    for(int i = 0; i<50; i++){
        char *randStr = malloc(20);
        randomStrings[i] = randomString(randStr);
        if(i <= 9){
            sprintf(randomStrings[i],"%s 0%d\n",randomStrings[i],i);
        }else{ 
            sprintf(randomStrings[i],"%s %d\n",randomStrings[i],i);
        }
        randomStrings[i][23]= '\0';
    }
    
    int ID = 0;
    while(ID<50){
        key1 = ftok("progfile1", 65);
        key2 = ftok("progfile2",75);

        msgid1 = msgget(key1, 0666 | IPC_CREAT);
        msgid2 = msgget(key2, 0666 | IPC_CREAT);
        message1->mesgType = 1;
        for(int i=ID;i<ID+5;i++){
            strcat(message1->mesgText,randomStrings[i]); 
        }
        msgsnd(msgid1, message1, sizeof(struct messageBuffer), 0);

        message1->mesgText[0] = '\0';
        message2->mesgText[0] = '\0';

        msgrcv(msgid2, message2, sizeof(struct messageBuffer), 2, 0);
        if(strcmp("",message2->mesgText) != 0)printf("Sent by Client: %s\n", message2->mesgText);
        ID = atoi(message2->mesgText) + 1;
        msgctl(msgid2, IPC_RMID, NULL);
    }
    return 0;
}