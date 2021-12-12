//got the serial version of the code from https://www.geeksforgeeks.org/sieve-of-eratosthenes/
//modified it using OMP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>

#define N 16
#define MAX_PRIME 1000
int arr[MAX_PRIME+1]={0};

int main(){
    struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
	int beginu=(int)current_time.tv_usec;
    omp_set_num_threads(N);
    int s=(int)(sqrt(MAX_PRIME));
    //#pragma omp parallel
    //{
        //#pragma omp parallel for
        for (int p = 2; p <= s; p++)
        {
            
            if (arr[p]==0)
            {
                #pragma omp parallel for    
                for (int i = p * p; i <= MAX_PRIME; i += p)
                    arr[i] = 1;
            }
        }
    //}
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