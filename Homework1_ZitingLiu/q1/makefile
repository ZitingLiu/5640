all: test1 memory

test1: linpack.c
	gcc -O3 -msse -o linpack linpack.c

memory: memory_test.c
	gcc -O3 -o memory_test memory_test.c