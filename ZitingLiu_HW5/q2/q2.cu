#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 16
#define n 64

__global__ void gpu_nn(float *a, float *b){
    int row = blockIdx.y * blockDim.y + threadIdx.y; 
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    //printf("Hello, world from block %d,%d\n", col, row);
    if(col>=1 && row>=1 && col<n-1 && row<n-1){
        for(int k=1;k<n-1;k++){
            a[row*n*n+col*n+k]=0.8*(b[(row-1)*n*n+col*n+k]+b[(row+1)*n*n+col*n+k]
            +b[row*n+(col+1)*n+k]+b[row*n+(col-1)*n+k]+b[row*n+col*n+k+1]+b[row*n+col*n+k]-1);
            //a[row*n*n+col*n+k]=1;
        }
    }
}




int main(int argc, char const *argv[]){
    srand(time(0));
    float a[n*n*n]={0};
    float b[n*n*n]={0};



    float *h_a,*h_b;
    cudaMallocHost((void **) &h_a, sizeof(float)*n*n*n);
    cudaMallocHost((void **) &h_b, sizeof(float)*n*n*n);

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                float temp=(float)(rand()%10)+1;
                //printf("%f ",temp);
                
                h_b[i*n*n+j*n+k]=temp;
                b[i*n*n+j*n+k]=temp;
                h_a[i*n*n+j*n+k]=0.0;
            }
        }
    }
    printf("\n");
    float gpu_elapsed_time_ms;

    // some events to count the execution time
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // start to count execution time of GPU version
    cudaEventRecord(start, 0);

    float *d_a,*d_b;
    cudaMalloc((void **) &d_a, sizeof(float)*n*n*n);
    cudaMalloc((void **) &d_b, sizeof(float)*n*n*n);

    cudaMemcpy(d_a,h_a, sizeof(float)*n*n*n,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b, sizeof(float)*n*n*n,cudaMemcpyHostToDevice);

    unsigned int grid = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(grid,grid);
    dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);

    gpu_nn<<<dimGrid,dimBlock>>>(d_a,d_b);
    
    cudaMemcpy(h_a, d_a, sizeof(float)*n*n*n, cudaMemcpyDeviceToHost);
    cudaMemcpy(h_b, d_b, sizeof(float)*n*n*n, cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();
    // time counting terminate
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
    printf("Elapsed time %f", gpu_elapsed_time_ms);
    printf("\n\n");
    
    
    ///////////////////////cpu version just to verify result
    for(int i=1;i<n-1;i++){
        for(int j=1;j<n-1;j++){
            for(int k=1;k<n-1;k++){
                a[i*n*n+j*n+k]=0.8*(b[(i-1)*n*n+j*n+k]+b[(i+1)*n*n+j*n+k]
            +b[i*n+(j+1)*n+k]+b[i*n+(j-1)*n+k]+b[i*n+j*n+k+1]+b[i*n+j*n+k]-1);
            }
        }
    }
    bool all_ok=true;
    int ind=0;
    for(int i=1;i<n-1;i++){
        for(int j=1;j<n-1;j++){
            for(int k=1;k<n-1;k++){
                if(h_a[i*n*n+j*n+k]!=a[i*n*n+j*n+k]){
                    all_ok=0;
                    ind=i*n*n+j*n+k;
                    break;
                } 
            }
        }
    }
    if(all_ok){
        printf("All results are correct!\n");
    }else{
        printf("something went wrong at index %d.\n",ind);
    }
    
    printf("CUDA     CPU\n");
    
    for(int i=1;i<n-1;i++){
        for(int j=1;j<n-1;j++){
            for(int k=1;k<n-1;k++){
                
                printf("%6.3f ",h_a[i*n*n+j*n+k]);
                printf("%6.3f ",a[i*n*n+j*n+k]);
                printf("\n");
            }
        }
    }

    printf("\n\n");

}

