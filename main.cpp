#include "mpi.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include "common.h"
#include "mainProgram.h"
#include "receiver.h"
#include "globals.h"
#include "communicator.h"
#include <time.h>
#include <stddef.h>


using namespace std;



int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr,"Requires at least two processes.\n");
        exit(-1);
    }

    /* create a type for struct message */
    const int itemsCount = 3;
    int blockLengths[itemsCount] = { 1, 1, 1};
    MPI_Datatype types[itemsCount] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype* mpi_message_type = getType();
    MPI_Aint offsets[itemsCount];

    offsets[0] = offsetof(Message, messageType);
    offsets[1] = offsetof(Message, processId);
    offsets[2] = offsetof(Message, timeStamp);

    MPI_Type_create_struct(itemsCount, blockLengths, offsets, types, mpi_message_type);
    MPI_Type_commit(mpi_message_type);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    setProcessId(rank);
    setProcessNumber(size);
    setPortalCapacity(1); //TODO add var from command prompt

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