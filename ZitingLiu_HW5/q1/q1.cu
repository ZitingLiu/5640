#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 128
#define NUM_INPUT 33554432
#define NUM_CLASS 8
#define BIN 1000000/NUM_CLASS

__global__ void binning(int* arr,int* result,int* example){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid>=NUM_INPUT) return;
    
    
    int temp=arr[tid];
    for(int j=0;j<NUM_CLASS;j++){
        if(temp>=(j*BIN) && temp<(j+1)*BIN){
            atomicAdd(&result[j],1);
            if(example[j]==0){
                example[j]=temp;
            } 
            break;
        }
    }
    

}

int main(int argc, char const *argv[]){
    printf("Making histogram for %d inputs.\n",NUM_INPUT);
    int *arr, *result,*example;
    cudaMallocHost((void **) &arr, sizeof(int)*NUM_INPUT);
    cudaMallocHost((void **) &result, sizeof(int)*NUM_CLASS);
    cudaMallocHost((void **) &example, sizeof(int)*NUM_CLASS);

    srand(time(0));
    for(int i=0;i<NUM_INPUT;i++){
        arr[i]=rand()%1000000;
        //printf("%d ",arr[i]);
    }
    for(int i=0;i<NUM_CLASS;i++){
        result[i]=0;
        example[i]=0;
    }

    float gpu_elapsed_time_ms;
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // start to count execution time of GPU version
    cudaEventRecord(start, 0);

    int *d_arr, *d_result, *d_example;
    cudaMalloc((void **) &d_arr, sizeof(int)*NUM_INPUT);
    cudaMalloc((void **) &d_result, sizeof(int)*NUM_CLASS);
    cudaMalloc((void **) &d_example, sizeof(int)*NUM_CLASS);

    cudaMemcpy(d_arr, arr, sizeof(int)*NUM_INPUT, cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, result, sizeof(int)*NUM_CLASS, cudaMemcpyHostToDevice);
    cudaMemcpy(d_example, example, sizeof(int)*NUM_CLASS, cudaMemcpyHostToDevice);
    
    unsigned int grid_rows = (NUM_INPUT + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(grid_rows);
    dim3 dimBlock(BLOCK_SIZE);

    binning<<<dimGrid,dimBlock>>>(d_arr,d_result,d_example);
    cudaMemcpy(example, d_example, sizeof(int)*NUM_CLASS, cudaMemcpyDeviceToHost);
    cudaMemcpy(result, d_result, sizeof(int)*NUM_CLASS, cudaMemcpyDeviceToHost);
    
    cudaThreadSynchronize();
    cudaDeviceSynchronize();
    // time counting terminate
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
    printf("Elapsed time: %f ms.\n\n", gpu_elapsed_time_ms);
    
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