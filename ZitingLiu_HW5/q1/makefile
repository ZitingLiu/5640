all: q1 q1_omp
 
q1: q1.cu
	nvcc q1.cu -o q1

q1_omp: q1_omp.c
	gcc -O -std=c99 -fopenmp -o q1_omp q1_omp.c
