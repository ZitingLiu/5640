all: taylor avx

taylor: taylor.c
	gcc -O -std=c99 -o taylor taylor.c -lm

avx: avx.cpp
	g++ -O3 -fopt-info-vec-all -ftree-vectorize -unroll-loops -g -mavx -march=native -mtune=native -o avx ./avx.cpp