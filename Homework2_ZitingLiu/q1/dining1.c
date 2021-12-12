/*
Learned from "Modern Operating Systems 4th edition by Andrew s Tanenbaum"
Chapter 2.5 : Classical Problems
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5  //define the number of philosophers
#define LEFT (n+N-1)%N
#define RIGHT (n+1)%N
#define THINKING 2
#define HUNGRY 1
#define EATING 0

sem_t semaphore;
sem_t S[N];
int status[N];
int mid=1;
int usingMiddle[N]={0};//1 using left and middle, 2 using right and middle

int getForkNum(int i){
    if(i==N-1) return 1;
    else return i+2;
}

void think(int n){
    printf("Philosopher %d is thinking.\n",n+1);
}



void test(int n){
    if(status[n]==HUNGRY && status[LEFT]!=EATING&&status[RIGHT]!=EATING){
        status[n]=EATING;
        int r = rand()%3;
        
        printf("Philosopher %d is eating. ",n+1);
        printf("Takes fork %d and %d\n", n+1,getForkNum(n));
        sleep(r);//eating takes random amount of time between 0 and 10 seconds
        sem_post(&S[n]);
    }else if(status[n]==HUNGRY && status[LEFT]!=EATING&&mid==1){
        status[n]=EATING;
        int r = rand()%3;
        mid=0;
        usingMiddle[n]=1;
        printf("Philosopher %d is eating. ",n+1);
        printf("Takes fork %d and middle fork\n", n+1);
        sleep(r);//eating takes random amount of time between 0 and 10 seconds
        sem_post(&S[n]);
    }else if(status[n]==HUNGRY && status[RIGHT]!=EATING&&mid==1){
        status[n]=EATING;
        int r = rand()%3;
        mid=0;
        usingMiddle[n]=2;
        printf("Philosopher %d is eating. ",n+1);
        printf("Takes fork %d and middle fork\n", getForkNum(n));
        sleep(r);//eating takes random amount of time between 0 and 10 seconds
        sem_post(&S[n]);
    }
}

void take(int n){
    sem_wait(&semaphore);
    status[n]=HUNGRY;
    printf("philosopfer %d is trying to take forks\n",n+1);
    test(n);
    sem_post(&semaphore);
    sem_wait(&S[n]);
}

void put(int n){
    sem_wait(&semaphore);
    status[n]=THINKING;
    if(usingMiddle[n]==1){
        usingMiddle[n]=0;
        mid=1;
        printf("Philosopher %d stopped eating and put down forks %d and middle fork\n",n+1,n+1);
        test((n+N-1)%N);//LEFT
        test((n+1)%N);//RIGHT
    }else if(usingMiddle[n]==2){
        usingMiddle[n]=0;
        mid=1;
        printf("Philosopher %d stopped eating and put down forks %d and middle fork\n",n+1,getForkNum(n));
        test((n+N-1)%N);//LEFT
        test((n+1)%N);//RIGHT
    }
    else{
        
        printf("Philosopher %d stopped eating and put down forks %d and %d\n",n+1,n+1,getForkNum(n));
        test((n+N-1)%N);//LEFT
        test((n+1)%N);//RIGHT
        
    }
    sem_post(&semaphore);
    
}



void phil(int n){
    while(1){
        think(n);
        take(n);
        put(n);
        sleep(1);
    }
}



int main(){
    


    pthread_t thread_id[N];
    srand(time(NULL));
    sem_init(&semaphore, 0, 1);
    for(int i=0;i<N;i++){
        sem_init(&S[i],0,0);
    }
    for(int i=0;i<N;i++){
        pthread_create(&thread_id[i], NULL, &phil, i);
    }
    sleep(10);
    return 0;
    for(int i=0;i<N;i++){
        pthread_join(thread_id[i], NULL);
    }
    
    

    sem_destroy(&semaphore);
    return 0;

}