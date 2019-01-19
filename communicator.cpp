#include <sstream>
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

    int buffer[10];
    MPI_Recv(&buffer, 3, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    ShowMessage(isMain, "after recive");

    message.processId = buffer[0];
    message.timeStamp = buffer[1];
    message.messageType = buffer[2];

    ShowMessage(isMain, "recived");

    UpdateLamportClock(message.timeStamp);

    return message;
}

void Send(int toProcess, Message message, int tag)
{
    UpdateLamportClock(0);
    std::ostringstream stringStream;
    stringStream << message.processId << "x" << message.timeStamp << "x" << message.messageType << std::endl;
    ShowMessage(false, stringStream.str());
    int buffer[10] = {message.processId, message.timeStamp, message.messageType, -1, -2, -3, -4, -5, -6, -7};
    MPI_Send(&buffer, 3, MPI_INT, toProcess, tag, MPI_COMM_WORLD);
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