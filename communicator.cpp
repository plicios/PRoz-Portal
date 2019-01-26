#include <sstream>
#include "communicator.h"
#include "mpi.h"
#include "common.h"
#include "utils.h"

#define messageSize 4

Message Receive(int tag, bool isMain)
{
    MPI_Status status;
//    ShowMessage(isMain, "before recive");
    Message message;

    int buffer[messageSize];
    MPI_Recv(&buffer, messageSize, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//    ShowMessage(isMain, "after recive");

    message.processId = buffer[0];
    message.timeStamp = buffer[1];
    message.messageType = buffer[2];
    message.data = buffer[3];

//    ShowMessage(isMain, "recived");

    UpdateLamportClock(message.timeStamp);

    return message;
}

void Send(int toProcess, Message message, int tag)
{
    std::ostringstream stringStream;
    stringStream << "send message: " << toProcess << " : " << message.timeStamp << " : " << message.messageType << " : " << message.data << std::endl;
    ShowMessage(false, stringStream.str());

    UpdateLamportClock(0);
    int buffer[messageSize] = {message.processId, message.timeStamp, message.messageType, message.data};
    MPI_Send(&buffer, messageSize, MPI_INT, toProcess, tag, MPI_COMM_WORLD);
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

void PortalGrantedGet(int requestTimestamp)
{
    Message message = Receive(PortalGrantedTag, true);
    if(message.timeStamp <= requestTimestamp)
    {
        PortalGrantedGet(requestTimestamp);
    }
}

void PortalGrantedSend()
{
    Message message;
    message.processId = getProcessId();
    message.timeStamp = GetLamportTime();
    message.messageType = -1;
    message.data = 0;
    Send(getProcessId(), message, PortalGrantedTag);
}