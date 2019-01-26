#include "mpi.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include "common.h"
#include "mainProgram.h"
#include "receiver.h"
#include "globals.h"
#include <time.h>
#include <stddef.h>


using namespace std;



int main(int argc, char** argv)
{
    int rank, size;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided!=MPI_THREAD_MULTIPLE) {
        MPI_Finalize();
        fprintf(stderr,"ERROR: The MPI library does not have full thread support.\n");
        exit(-2);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr,"Requires at least two processes.\n");
        exit(-1);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    setProcessId(rank);
    setProcessNumber(size);
    setPortalCapacity(atoi(argv[1]));

    srand((unsigned int) (time(0) + rank));

    int rc1;

    pthread_t thread1;
    //Create receiver thread
    if((rc1=pthread_create( &thread1, NULL, &receiver, NULL)))
    {
        printf("Thread creation failed: %d\n", rc1);
    }
    else
    {
        mainProgram();
    }

    MPI_Finalize();
    return 0;
}
