#include "communicator.h"
#include "mpi.h"
#include "common.h"
#include "utils.h"

MPI_Datatype mpi_message_type;

MPI_Datatype* getType()
{
    return &mpi_message_type;
}


Message Receive(int tag, bool isMain)
{
    MPI_Status status;
    ShowMessage(isMain, "before recive");
    Message message;
    MPI_Recv(&message, 1, mpi_message_type, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);

    ShowMessage(isMain, "recived");

    UpdateLamportClock(message.timeStamp);

    return message;
}

void Send(int toProcess, Message message, int tag)
{
    UpdateLamportClock(0);
    MPI_Send(&message, 1, mpi_message_type, toProcess, tag, MPI_COMM_WORLD);
}

void SendToAll(Message message, int tag)
{
    for (int i = 0; i < getProcessNumber(); i++)
    {
        if(i != getProcessId())
        {
            Send(i, message, tag);
        }
    }
}

void PortalGrantedGet()
{
    Receive(PortalGrantedTag, true);
}

void PortalGrantedSend()
{
    Message message;
    message.processId = getProcessId();
    message.timeStamp = GetLamportTime();
    message.messageType = -1;
    Send(getProcessId(), message, PortalGrantedTag);
}