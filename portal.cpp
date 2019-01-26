#include "portal.h"

int status;
int responseCounter;
int myRequestTimestamp = -1;
int inPortal;
int myLocation; //0 - down, 1 - up

//add mutexes

void ChangeLocation()
{
    myLocation = 1 - myLocation;
}

int GetMyDirection()
{
    return myLocation == 0 ? 1 : -1;
}

void ClearPortalStatus()
{
    status = 0;
    responseCounter = 0;
}

void UpdatePortalStatus(int newPortalTaken)
{
    status += newPortalTaken;
}

int GetPortalStatus()
{
    return status;
}

void IncrementResponseCounter()
{
    responseCounter++;
}

int GetResponseCounter()
{
    return responseCounter;
}

int GetMyRequestTimestamp()
{
    return myRequestTimestamp;
}

void SetMyRequestTimestamp(int _myRequestTimestamp)
{
    myRequestTimestamp =_myRequestTimestamp;
}

void SetInPortal(int _inPortal)
{
    inPortal = _inPortal;
}

int GetInPortal()
{
    return inPortal;
}
