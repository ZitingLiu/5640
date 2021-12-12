#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 16
#define N 10000

__global__ void gpu_dart(int* a,int* result){
    int row = blockIdx.y * blockDim.y + threadIdx.y; 
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if(row<N && col<N){
        //printf("%d\n",a[N*row+col]);
        if(a[N*row+col]<314) result[N*row+col]=1;
    }

}




int main(int argc, char const *argv[]){
    int *h_a,*h_total;
    cudaMallocHost((void **) &h_a, sizeof(int)*N*N);
    cudaMallocHost((void **) &h_total, sizeof(int)*N*N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int temp= rand() % 400;
            h_a[i * N + j]=temp; 
            h_total[i * N + j]=0; 
            
        }
    }

    int *d_a, *d_total;
    cudaMalloc((void **) &d_a, sizeof(int)*N*N);
    cudaMalloc((void **) &d_total, sizeof(int)*N*N);

    cudaMemcpy(d_a, h_a, sizeof(int)*N*N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_total, h_total, sizeof(int)*N*N, cudaMemcpyHostToDevice);

    unsigned int grid = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(grid, grid);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

    gpu_dart<<<dimGrid,dimBlock>>>(d_a,d_total);

    cudaMemcpy(h_total, d_total, sizeof(int)*N*N, cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();
    int sum=0;
    for(int i=0;i<N*N;i++) {
        if(h_total[i]==1) sum++;
    }
    double pi=(double)sum/(double)(N*N)*4.0;
    printf("Estimated result with %d inputs : %f\n",N*N,pi);


}