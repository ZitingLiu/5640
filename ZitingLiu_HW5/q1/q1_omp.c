#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>

#define NUM_INPUT 33554432
#define NUM_CLASS 8
#define BIN 1000000/NUM_CLASS


int main(){
    printf("Making histogram for %d inputs.\n",NUM_INPUT);
    srand(time(0));
    int arr[NUM_INPUT]={0};
    for(int i=0;i<NUM_INPUT;i++){
        arr[i]=rand()%1000000;
    }
    int result[NUM_CLASS]={0};
    int example[NUM_CLASS]={0};
    
    struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
    int beginu=(int)current_time.tv_usec;
#pragma omp paralle for  
    for(int i=0;i<NUM_INPUT;i++){
        int temp=arr[i];
        for(int j=0;j<NUM_CLASS;j++){
            if(temp>=j*BIN && temp<(j+1)*BIN){
                result[j]++;
                if(example[j]==0) example[j]=temp;
                break;
            }   
        }

    }
   



    gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin*1000000+beginu;
	double endtime=end*1000000+endu;
    
    printf("\n Total time : %f ms\n",(endtime-begintime)/1000);
    printf("Number of elements in each of the %d bins are: \n",NUM_CLASS);
    for(int i=0;i<NUM_CLASS;i++){
        printf("%d ",result[i]);
    }
    printf("\n");

    printf("One number from each of the %d bins to show that result is correct \n",NUM_CLASS);
    for(int i=0;i<NUM_CLASS;i++){
        printf("%d ",example[i]);
    }
    printf("\n");
    return 0;
}