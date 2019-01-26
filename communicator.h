#include "mpi.h"
#include "globals.h"

Message Receive(int tag, bool isMain);
void Send(int toProcess, Message message, int tag);
void SendToAll(Message message, int tag);

void PortalGrantedGet(int requestTimestamp);
void PortalGrantedSend();