//got the serial version of the code from https://www.geeksforgeeks.org/sieve-of-eratosthenes/
//modified it using pthread

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_PRIME 1000
#define THREADNUM 16
int arr[MAX_PRIME+1]={0};
pthread_mutex_t mutex;


void removeNum(int t){
    
    for(int i=t*t;i<=MAX_PRIME;i+=t){
        arr[i]=1;
    }
    
}   




int main(){
    pthread_t thread_id[THREADNUM];
    struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
	int beginu=(int)current_time.tv_usec;
    int tcount=0;
    pthread_mutex_init(&mutex,NULL);
    for(int p=2;p*p<=MAX_PRIME;p++){
        if(arr[p]==0){
            if(tcount<THREADNUM){
                pthread_create(&thread_id[tcount], NULL, &removeNum, p);
                tcount++;
            }else{
                for(int i=0;i<THREADNUM;i++){
                    if(pthread_join(thread_id[i], NULL)==0){
                            pthread_create(&thread_id[i], NULL, &removeNum,p);
                    }
                }
            //removeNum(p);
            }

        }
        
    }
    gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin+beginu*0.000001;
	double endtime=end+endu*0.000001;


    for(int i=2;i<=MAX_PRIME;i++){
        if(!arr[i]) printf("%d ",i);
    }
    printf("\n");
    printf("overall time taken:%f\n",endtime-begintime);

    return 0;
}