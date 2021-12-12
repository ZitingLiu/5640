#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <time.h>
#include <sys/time.h>

#define NUM_NODES 2
#define NUM_DARTS 20000
void main(int argc, char *argv[]){
    struct timeval current_time;
  	gettimeofday(&current_time, NULL);
	int begin=(int)current_time.tv_sec;
    int beginu=(int)current_time.tv_usec;

    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);
    srand(rank+1);
    
    if(rank==0){
        printf("Processing %d darts with %d nodes.\n",NUM_NODES*NUM_DARTS,NUM_NODES);
        int count=0;
        for (int i = 0; i < NUM_DARTS; i++) {
            int temp = rand() % 400;
            if (temp < 314) count++;
        }
        double ratio = (double)count / NUM_DARTS;
        ratio = ratio * 4.0;
        double ratioArr[NUM_NODES]={0.0};
        //ratioArr[0]=ratio;
        double sum=ratio;
        //printf("%f\n",ratio);
        //printf("%d before receiving\n",rank);
        for(int i=1;i<NUM_NODES;i++){
            //printf("receiving %d\n",i);
            double temp=0.0;
            MPI_Recv(&temp, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //ratioArr[i]=temp;
            sum+=temp;
            //printf("Pi received from %d is %f\n",i,temp);
        }
        //printf("%d after receiving\n",rank);
        double result=sum/NUM_NODES;
        printf("Pi is %f\n",result);
        
    }else{
        //printf("%d before sending\n",rank);
        int count=0;
        for (int i = 0; i < NUM_DARTS; i++) {
            int temp = rand() % 400;
            if (temp < 314) count++;
        }
        double ratio = (double)count / NUM_DARTS;
        ratio = ratio * 4.0;
        MPI_Send(&ratio, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        //printf("%d after sending\n",rank);
    }
    gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin+beginu*0.000001;
	double endtime=end+endu*0.000001;
    printf("Node %d finished in %f\n",rank,endtime-begintime);

    MPI_Finalize();
}