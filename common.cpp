#include <pthread.h>
#include "common.h"

int processId;
int processNumber;
bool executeProgram = true;

int getProcessId()
{
    return processId;
}
bool getExecuteProgram()
{
    return executeProgram;
}
int getProcessNumber()
{
    return processNumber;
}

void setProcessId(int _processId)
{
    processId = _processId;
}
void setExecuteProgram(bool _executeProgram)
{
    executeProgram = _executeProgram;
}
void setProcessNumber(int _processNumber)
{
    processNumber = _processNumber;
}

int portalCapacity;

int getPortalCapacity()
{
    return portalCapacity;
}

void setPortalCapacity(int _portalCapacity)
{
    portalCapacity = _portalCapacity;
}

bool waitingForPortal = false;
pthread_mutex_t waitingForPortalMutex = PTHREAD_MUTEX_INITIALIZER;

bool getWaitingForPortal()
{
    pthread_mutex_lock(&waitingForPortalMutex);
    bool returnValue = waitingForPortal;
    pthread_mutex_unlock(&waitingForPortalMutex);
    return returnValue;
}

void setWaitingForPortal(bool _waitingForPortal)
{
    pthread_mutex_lock(&waitingForPortalMutex);
    waitingForPortal = _waitingForPortal;
    pthread_mutex_unlock(&waitingForPortalMutex);
}