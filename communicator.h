#include "mpi.h"
#include "globals.h"

MPI_Datatype* getType();
Message Receive(int tag, bool isMain);
void Send(int toProcess, Message message, int tag);
void SendToAll(Message message, int tag);

void PortalGrantedGet();
void PortalGrantedSend();