all:dining_omp dining

dining_omp: dining_omp.c
	gcc -O -std=c99 -fopenmp -o dining_omp dining_omp.c

dining: dining.c
	gcc -O -std=c99 -pthread -o dining dining.c