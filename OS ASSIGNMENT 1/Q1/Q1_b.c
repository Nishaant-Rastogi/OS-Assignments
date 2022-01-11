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
#include <pthread.h>

extern int errno;

void print(char *str){
    size_t w = write(0, str, strlen(str));
    if (w < 0){
        if (errno != EINTR){
            perror("write");
            return;
        }
    }
}
typedef struct thread_args {
    double (*students)[1];
    double (*assgn)[6];
} thread_args;
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
void* threadStartA(void* arg){
    thread_args *targs = (thread_args *)arg;
    int studentId[10000];
    char *section[10000];
    int marks[6][10000];
    double avg = 0;
    char str[1000];
    int rowCount = dataProcessing(studentId, section, marks);
    *targs->students[0] = rowCount;
    for(int i = 0; i < 6; i++){
        avg = 0;
        int strength = 0;
        for (int j = 0; j < rowCount; j++){
            if(*section[j] == 'A'){
                avg += marks[i][j];
                *targs->assgn[i] += marks[i][j];
                strength++;
            }
        }
        avg /= strength;
        snprintf(str, 100, "%d) Assignment %d average marks in section %c : %lf\n", i+1, i+1, 'A', avg);
        print(str);
    }
    print("\n");
}
void* threadStartB(void* arg){
    thread_args *targs = (thread_args *)arg;
    int studentId[10000];
    char *section[10000];
    int marks[6][10000];
    double avg = 0;
    char str[1000];
    int rowCount = dataProcessing(studentId, section, marks);
    *targs->students[0] = rowCount;
    for(int i = 0; i < 6; i++){
        avg = 0;
        int strength = 0;
        for (int j = 0; j < rowCount; j++){
            if(*section[j] == 'B'){
                avg += marks[i][j];
                *targs->assgn[i] += marks[i][j];
                strength++;
            }
        }
        avg /= strength;
        snprintf(str, 100, "%d) Assignment %d average marks in section %c : %lf\n", i+1, i+1, 'B', avg);
        print(str);
    }
    print("\n");
}
int main(){
    pthread_t pThread1;
    pthread_t pThread2;
    thread_args targs;
    double students[] = {0};
    double assignment[] = {0,0,0,0,0,0};
    targs.students = &students;
    targs.assgn = &assignment;
    if(pthread_create(&pThread1, NULL, &threadStartA, (void *)&targs) != 0){
        perror("pthread_create");
        return -1;
    }
    if(pthread_create(&pThread2, NULL, &threadStartB, (void *)&targs) != 0){
        perror("pthread_create");
        return -1;
    }
    if(pthread_join(pThread1, NULL) != 0){
        perror("pthread_join");
        return -1;
    }
    if(pthread_join(pThread2, NULL) != 0){
        perror("pthread_join");
        return -1;
    }

    double avg = 0;
    char str[100];
    for(int i = 0; i<6; i++){
        assignment[i] = *targs.assgn[i];
    }
    students[0] = *targs.students[0];
    for(int i = 0; i<6; i++){
        avg = assignment[i]/(students[0]);
        snprintf(str, 100, "%d) The average marks of Assignment %d across sections : %lf\n", i+1, i+1, avg);
        print(str);
    }
    pthread_exit(NULL);
}