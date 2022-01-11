#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t bowls;
sem_t forks[5];

void eat(int philo){
	printf("Philosopher %d started eating\n",philo);
}

void * philosopherDining(void * number){   
	while(1){
		int philo = *(int *)number;
		printf("Philosopher %d is hungry\n",philo+1);
		sem_wait(&bowls);
		printf("Philosopher %d got a Bowl\n",philo+1);
		sem_wait(&forks[philo]);
		sem_wait(&forks[(philo+1)%5]);
		printf("Philosopher %d got Forks\n",philo+1);
		eat(philo);
		sleep(2);
		printf("Philosopher %d has finished eating\n",philo+1);
		sem_post(&forks[philo]);
		sem_post(&forks[(philo+1)%5]);
		sem_post(&bowls);
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
	sem_init(&bowls,0,4);
	forksWait();
	philoThreadCreate(philosophers, thread_ID);
	philoThreadJoin(thread_ID);
}