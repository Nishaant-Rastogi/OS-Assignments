#include <stdlib.h>
#include <stdio.h>
extern void B(long long argument);

void A(){
    long long argument;
    printf("%s\n","A() is executing.");
    printf("Enter 64 bit integer: ");
    scanf("%lli", &argument);
    B(argument);
}

int main(void){
    A();
}