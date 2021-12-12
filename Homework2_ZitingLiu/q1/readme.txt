make file will compile everything

dining.c is the standard one that takes odd number of inputs, to change the
number of philosophers, please use the define at line 14
it will print out the actions of philosophers for 10 seconds, to observe more
actions, please change the sleep time at line 92. For dining1.c and dining3.c
please use the same method for different inputs

dining1.c is the extra credit code for part a. It has a shared middle fork for
all philosophers to use. But due to some printing order issue, philosopher sometimes 
will take the middle fork directly.I don't know how to fix this problem.
But I think the implementation is correct.

dining 3 is the one where all philosophers take the same time to eat. 
This was an easy modification, and no obvious changes are there in the result.