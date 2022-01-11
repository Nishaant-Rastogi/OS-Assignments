#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t forks[5];

void eat(int philo){
	printf("Philosopher %d is eating\n",philo+1);
}

void * philosopherDining(void * num){
	while(1){
		int philo = *(int *)num;
		if(philo >= 4){
			printf("Philosopher %d is hungry\n",philo+1);
			sem_wait(&forks[(philo+1)%5]);
			sem_wait(&forks[philo]);
			printf("Philosopher got Forks\n",philo+1);
			eat(philo);
			sleep(2);
			printf("Philosopher %d has finished eating\n",philo+1);
			sem_post(&forks[(philo+1)%5]);
			sem_post(&forks[philo]);
		}else{
			printf("Philosopher %d is hungry\n",philo+1);
			sem_wait(&forks[philo]);
			sem_wait(&forks[(philo+1)%5]);
			printf("Philosopher got Forks\n",philo+1);
			eat(philo);
			sleep(2);
			printf("Philosopher %d has finished eating\n",philo+1);
			sem_post(&forks[philo]);
			sem_post(&forks[(philo+1)%5]);
		}
	}
}
void forksWait(void){
	for(int i=0;i<5;i++){
		sem_init(&forks[i],0,1);
	}
}
void philoThreadCreate(int philosophers[], pthread_t thread_ID[]){
	for(int i=0;i<5;i++){
		philosophers[i]=i;
		pthread_create(&thread_ID[i],NULL,philosopherDining,(void *)&philosophers[i]);
	}
}
void philoThreadJoin(pthread_t thread_ID[]){
	for(int i=0;i<5;i++){
		pthread_join(thread_ID[i],NULL);
	}
}
int main(){
	int philosophers[5];
	pthread_t thread_ID[5];
	forksWait();
	philoThreadCreate(philosophers, thread_ID);
	philoThreadJoin(thread_ID);
}