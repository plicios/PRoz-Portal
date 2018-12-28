#include <zconf.h>
#include <pthread.h>
#include <vector>
#include "globals.h"
#include "portal.h"
#include "common.h"

int portalAgrees;
pthread_mutex_t portalAgreesMutex = PTHREAD_MUTEX_INITIALIZER;

int myRequestTimeStamp = -1;
pthread_mutex_t myRequestTimeStampMutex = PTHREAD_MUTEX_INITIALIZER;

std::string myDirection;
pthread_mutex_t myDirectionMutex = PTHREAD_MUTEX_INITIALIZER;

std::vector<PortalPlace> portalPlaces = std::vector<PortalPlace>();
pthread_mutex_t portalPlacesMutex = PTHREAD_MUTEX_INITIALIZER;

void IncrementAgrees()
{
    pthread_mutex_lock(&portalAgreesMutex);
    portalAgrees++;
    pthread_mutex_unlock(&portalAgreesMutex);
}
void ResetAgrees()
{
    pthread_mutex_lock(&portalAgreesMutex);
    portalAgrees = 1;
    pthread_mutex_unlock(&portalAgreesMutex);
}
int GetAgrees()
{
    pthread_mutex_lock(&portalAgreesMutex);
    int returnValue = portalAgrees;
    pthread_mutex_unlock(&portalAgreesMutex);
    return returnValue;
}

void AddToPortal(int processId, int timeStamp, std::string direction)
{
    PortalPlace portalPlace;
    portalPlace.timeStamp = timeStamp;
    portalPlace.processId = processId;
    portalPlace.direction = direction;

    pthread_mutex_lock(&portalPlacesMutex);
    portalPlaces.push_back(portalPlace);
    pthread_mutex_unlock(&portalPlacesMutex);
}

void RemoveFromPortal(int processId)
{
    pthread_mutex_lock(&portalPlacesMutex);
    for (int i = 0; i < portalPlaces.size(); i++)
    {
        if(portalPlaces[i].processId == processId)
        {
            portalPlaces.erase(portalPlaces.begin() + i);
            break;
        }
    }
    pthread_mutex_unlock(&portalPlacesMutex);
}

int GetPeopleBeforeMe()
{
    int peopleBeforeMe = 0;
    pthread_mutex_lock(&portalPlacesMutex);
    pthread_mutex_lock(&myRequestTimeStampMutex);
    for(int i = 0; i < portalPlaces.size(); i++)
    {
        PortalPlace processPolitics = portalPlaces[i];
        if(processPolitics.timeStamp < myRequestTimeStamp)
        {
            peopleBeforeMe += 1;
        }
        else if (processPolitics.timeStamp == myRequestTimeStamp && processPolitics.processId < getProcessId())
        {
            peopleBeforeMe += 1;
        }
    }
    pthread_mutex_unlock(&myRequestTimeStampMutex);
    pthread_mutex_unlock(&portalPlacesMutex);
    return peopleBeforeMe;
}

int GetMyPortalRequestTimeStamp()
{
    pthread_mutex_lock(&myRequestTimeStampMutex);
    int returnValue = myRequestTimeStamp;
    pthread_mutex_unlock(&myRequestTimeStampMutex);
    return returnValue;
}

void SetMyPortalRequestDirection(std::string _myDirection)
{
    pthread_mutex_lock(&myDirectionMutex);
    myDirection = _myDirection;
    pthread_mutex_unlock(&myDirectionMutex);
}

std::string GetMyPortalRequestDirection()
{
    pthread_mutex_lock(&myDirectionMutex);
    std::string returnValue = myDirection;
    pthread_mutex_unlock(&myDirectionMutex);
    return returnValue;
}

std::string GetPortalDirection()
{
    std::string returnValue;
    pthread_mutex_lock(&portalPlacesMutex);
    if(portalPlaces.empty())
    {
        returnValue = "no";
    }
    else
    {
        returnValue = portalPlaces.at(0).direction;
    }

    pthread_mutex_unlock(&portalPlacesMutex);
    return returnValue;
}