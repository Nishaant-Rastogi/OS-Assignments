#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sysexits.h>

extern int errno;

int dataProcessing(int studentId[10000], char *section[10000], int marks[6][10000]){
    int fileId = open("student_records.csv", O_RDONLY | O_EXCL);
    if(fileId == -1){
        perror("open");
        return -1;
    }
    char *records = (char *)malloc(10000 * sizeof(char));
    char *data[10000];
    int size = read(fileId, records, 10000);
    if(size < 0){
        perror("read");
        return -1;
    }
    char *token = strtok(records, "\n");
    int rows = 0;
    while (token != NULL) {
        data[rows++] = token;
        token = strtok(NULL, "\n");
    }
    for (int i = 0; i < rows-1; i++) {
        token = strtok(data[i + 1], ",");
        studentId[i] = atoi(token);
        token = strtok(NULL, ",");
        section[i] = token;
        token = strtok(NULL, ",");
        marks[0][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[1][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[2][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[3][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[4][i] = atoi(token);
        token = strtok(NULL, ",");
        marks[5][i] = atoi(token);
    }
    if (close(fileId) < 0) {
        perror("close");
        return -1;
    }
    free(records);
    return rows-1;
}

void computation(char *section[10000], int marks[6][10000], char sec,int rows){
    double avg = 0;
    char str[1000];
    for(int i = 0; i < 6; i++){
        avg = 0;
        int strength = 0;
        for (int j = 0; j < rows; j++){
            if(*section[j] == sec){
                avg += marks[i][j];
                strength++;
            }
        }
        avg /= strength;
        snprintf(str, 100, "%d) Assignment %d average marks in section %c : %lf\n", i+1, i+1, sec, avg);
        size_t wSize = write(0, str, strlen(str));
        if (wSize < 0){
            if (errno != EINTR){
                perror("write");
                return;
            }
        }
    }
    write(0, "\n", strlen("\n"));
}

int main()
{
    int studentId[10000];
    char *section[10000];
    int marks[6][10000];
    pid_t id = fork();
    int stat;
    if (id == 0){   
        int rows = dataProcessing(studentId, section, marks);
        computation(section, marks, 'A', rows);
        exit(1);
    }
    else if (id > 0) {
        int pid = waitpid(id, &stat, 0);
        if(pid == -1){
            perror("waitpid");
            return -1;
        }
        int rows = dataProcessing(studentId, section, marks);
        computation(section, marks, 'B', rows);
    }
    else{
        perror("fork");
        return -1;
    }
    return 0;
}