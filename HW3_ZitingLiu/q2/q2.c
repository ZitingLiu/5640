#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>

#define THREADNUM 8
#define MAX 100000
sem_t semaphore;
int num[MAX]={0};

void remove3(int n){
    int i=n;
    while(i<MAX){
        if(i%3==0){
            
            sem_wait(&semaphore);
            num[i]=1;
            sem_post(&semaphore);
        }
        i+=THREADNUM/2;
    }
}

void remove7(int n){
    int i=n;
    while(i<MAX){
        if(i%7==0){
            sem_wait(&semaphore);
            num[i]=1;
            sem_post(&semaphore);
        }
        i+=THREADNUM/2;
    }
}

int main(){
    struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
    int beginu=(int)current_time.tv_usec;

    pthread_t th[THREADNUM];
    sem_init(&semaphore,0,1);

    

    int t=THREADNUM/2;
    for(int i=0;i<t;i++){
        pthread_create(&th[i],NULL,&remove3,i);
    }
    for(int i=t;i<THREADNUM;i++){
        pthread_create(&th[i],NULL,&remove7,i-t);
    }

    for(int i=0;i<THREADNUM;i++){
        pthread_join(th[i],NULL);
    }
    
    
    gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin+beginu*0.000001;
	double endtime=end+endu*0.000001;

    for(int i=0;i<MAX;i++){
        if(num[i]==1) printf("%d ",i);
    }
    printf("\n Total time : %f\n",endtime-begintime);
    return 0;
}