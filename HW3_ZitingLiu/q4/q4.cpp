#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>

using namespace std;

#define SIZE 512

void printArr(int arr[]){
    for(int i=0;i<SIZE;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

int main(){
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%ld\n",number_of_processors);
    int nthreads, tid;
#pragma omp parallel private(nthreads, tid)
    {

    tid = omp_get_thread_num();
    printf("Thread ID : %d\n", tid);

    if (tid == 0) 
        {
        nthreads = omp_get_num_threads();
        printf("Number of threads = %d\n", nthreads);
        }

    }

    int a[SIZE][SIZE]={0};
    int b[SIZE]={0};
    int c[SIZE]={0};
    //cout<<"test"<<endl;
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            a[i][j]=rand()%10;
        }
        b[i]=rand()%10;
    }
#pragma omp parallel for
    for(int i=0;i<SIZE;i++){
        int sum=0;
#pragma omp parallel for reduction(+:sum)
        for(int j=0;j<SIZE;j++){
            sum+=a[i][j]*b[j];
        }
        c[i]=sum;
    }
    //Uncomment this section to see Martix A and Vector B
    /*cout<<"Matrix A is :"<<endl;
    for(int i=0;i<SIZE;i++){
        printArr(a[i]);
    }
    cout<<endl;
    cout<<"Vector B is :"<<endl;
    printArr(b);
    cout<<endl;*/
    cout<<"C=A*B, C is :"<<endl;
    printArr(c);
    cout<<endl;


    return 0; 
}