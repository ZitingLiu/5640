#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <time.h>
#include <sys/time.h>

#define NUM_NODES 2
#define NUM_CLASS 4
#define NUM_INPUT 10000
#define RANGE NUM_INPUT/NUM_CLASS
#define BIN 1000/NUM_CLASS

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
    int arr[NUM_CLASS]={0};

    if(rank==0){
        printf("Making histogram that has %d classes using %d nodes\n",NUM_CLASS,NUM_NODES);
        int histogram[NUM_CLASS]={0};
        if(NUM_CLASS<=NUM_NODES){
            for(int i=0;i<NUM_INPUT;i++){
                int value=rand()%1000;
                if(value>=0&& value<BIN){
                    histogram[0]++;
                }
            }
            //receive from other node
            for(int i=1;i<NUM_CLASS;i++){
                int temp=0;
                MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                histogram[i]+=temp;
            }
            for(int i=0;i<NUM_CLASS;i++){
                printf("%d ",histogram[i]);
            }
            printf("\n");

        }else{
            // run this to tell how many class each node needs to run
            int howmany[NUM_NODES] ={0};
            int remain=NUM_CLASS;
            while(remain>0){
                if(remain>NUM_NODES){
                    for(int i=0;i<NUM_NODES;i++){
                        howmany[i]++;
                    }
                    remain-=NUM_NODES;
                }else{
                    for(int i=0;i<remain;i++){
                        howmany[i]++;
                    }
                    remain=0;
                }
            }
            //for(int i=0;i<NUM_NODES;i++) printf("%d ",howmany[i]);
            //printf("\n");
            ///////////////////////////////////////////end of howmany
            int index=0;
            for(int i=0;i<howmany[0];i++){
                
                for(int j=0;j<NUM_INPUT;j++){
                    int value =rand()%1000;
                    if(value>=BIN*index && value<BIN*(index+1)){
                        histogram[index]++;
                    }
                }
                index++;
            }
            //receive
            int* test=malloc(NUM_CLASS*sizeof(int));
            for(int i=1;i<NUM_NODES;i++){
                MPI_Recv(test, NUM_CLASS, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(int j=0;j<NUM_CLASS;j++){
                    histogram[j]+=test[j];
                }
            }
            for(int i=0;i<NUM_CLASS;i++){
                printf("%d ",histogram[i]);
            }
            printf("\n");

        }
    }else{
        if(NUM_CLASS<=NUM_NODES){
            if(rank<NUM_CLASS){
                int total=0;
                for(int i=0;i<NUM_INPUT;i++){
                    int value=rand()%1000;
                    if(value>=rank*BIN&&value<(rank+1)*BIN) total++;
                }
                MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }


        }else{
            // run this to tell how many class each node needs to run
            int howmany[NUM_NODES] ={0};
            int remain=NUM_CLASS;
            while(remain>0){
                if(remain>NUM_NODES){
                    for(int i=0;i<NUM_NODES;i++){
                        howmany[i]++;
                    }
                    remain-=NUM_NODES;
                }else{
                    for(int i=0;i<remain;i++){
                        howmany[i]++;
                    }
                    remain=0;
                }
            }
            //for(int i=0;i<NUM_NODES;i++) printf("%d ",howmany[i]);
            //printf("\n");
            ///////////////////////////////////////////end of howmany
            int index=0;
            int histogram[NUM_CLASS]={0};
            for(int i=0;i<rank;i++){
                index+=howmany[i];
            }
            for(int i=0;i<howmany[rank];i++){
                for(int j=0;j<NUM_INPUT;j++){
                    int value =rand()%1000;
                    if(value>=BIN*index && value<BIN*(index+1)){
                        histogram[index]++;
                    }
                }
                index++;
            }
            MPI_Send(histogram, NUM_CLASS, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }



    gettimeofday(&current_time, NULL);
	int end=(int)current_time.tv_sec;
	int endu=(int)current_time.tv_usec;
	double begintime=begin+beginu*0.000001;
	double endtime=end+endu*0.000001;
    printf("Node %d finished in %f\n",rank,endtime-begintime);

    MPI_Finalize();
}