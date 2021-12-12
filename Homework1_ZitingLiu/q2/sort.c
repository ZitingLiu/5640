/* C program for Merge Sort */
//mergesort sourcecode taken from https://www.geeksforgeeks.org/merge-sort/
//plus modifications for multi-thread
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
# include <pthread.h>

#define THREAD 8

pthread_mutex_t mutex;

int part=0;
int arr[5000];
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int l, int m, int r)
{	
	//pthread_mutex_lock(&mutex);
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
	//pthread_mutex_unlock(&mutex);
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int l, int r)
{
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l + (r - l) / 2;

		// Sort first and second halves
		mergeSort( l, m);
		mergeSort( m + 1, r);

		merge( l, m, r);
	}
}

void* merge_sort(void* arg){
		
	int threadnum=part;
	part++;

	int low = threadnum * (5000 / THREAD);
    int high = (threadnum + 1) * (5000 / THREAD) - 1;
 
    // evaluating mid point
    int mid = low + (high - low) / 2;
	
    if (low < high) {
        mergeSort(low, mid);
        mergeSort( mid + 1, high);
		
        merge(low, mid, high);
    }


}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}

/* Driver code */
int main()
{
	pthread_mutex_init(&mutex,NULL);
	//recore begin time
	struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
	int beginu=(int)current_time.tv_usec;
	
	//generate rand int array
	
	int arr_size =5000;
	for(int i=0;i<5000;i++){
		arr[i]=rand()%100000;
	}
	
	//Generate threads
	pthread_t threads[THREAD];
	for (int i = 0; i < THREAD; i++){
		pthread_create(&threads[i], NULL, merge_sort, (void*)NULL);
	}
	for (int j = 0; j < THREAD; j++){
		pthread_join(threads[j], NULL);
	}


	if(THREAD==2){
		merge(0, (5000 - 1)/2, 4999);
	}
	else if(THREAD==4){
		merge(0, (5000 / 2 - 1) / 2, 5000 / 2 - 1);
    	merge(5000 / 2, 5000/2 + (5000-1-5000/2)/2, 4999);
    	merge(0, (5000 - 1)/2, 4999);
	}else if(THREAD==8){
		merge(0,1249/2,1249);
		merge(1250,1250+(2499-1250)/2,2499);
		merge(2500,2500+(3749-2500)/2,3749);
		merge(3750,3750+(4999-3750)/2,4999);
		merge(0, (5000 / 2 - 1) / 2, 5000 / 2 - 1);
    	merge(5000 / 2, 5000/2 + (5000-1-5000/2)/2, 4999);
    	merge(0, (5000 - 1)/2, 4999);
	}
	
	//record end time and print time 
	gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin+beginu*0.000001;
	double endtime=end+endu*0.000001;
       
	printArray(arr, arr_size);
	pthread_mutex_destroy(&mutex);
	
	printf("overall time taken:%f\n",endtime-begintime);

	return 0;
}
