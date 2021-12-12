all: prime prime_omp

prime: prime.c
	gcc -O -std=c99 -pthread -o prime prime.c

prime_omp: prime_omp.c
	gcc -O -std=c99 -fopenmp -o prime_omp prime_omp.c

