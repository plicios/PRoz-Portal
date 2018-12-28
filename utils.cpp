#include "utils.h"
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <zconf.h>
#include "common.h"

using namespace std;

void Wait(int minWaitTime, int maxWaitTime)
{
    int randomTime = rand() % (maxWaitTime - minWaitTime) + minWaitTime;
    ostringstream startStringStream;
    startStringStream << "start sleep " << randomTime << " milliseconds";
    ShowMessage(true, startStringStream.str());
    usleep(randomTime * 1000);
    ostringstream stopStringStream;
    stopStringStream << "stop sleep " << randomTime << " milliseconds";
    ShowMessage(true, stopStringStream.str());
}

int lamportTime = 0;
pthread_mutex_t lamportTimeMutex = PTHREAD_MUTEX_INITIALIZER;

void ShowMessage(bool isMain, string message)
{
    string threadName;
    if(isMain)
    {
        threadName = "Main";
    }
    else
    {
        threadName = "Listener";
    }
    std::cout << getProcessId() << ":" << std::setfill('0') << std::setw(5) << lamportTime << ":" << threadName << " || " << message << std::endl;
}


void UpdateLamportClock(int otherProcessTimeStamp)
{
    pthread_mutex_lock(&lamportTimeMutex);
    lamportTime = (lamportTime > otherProcessTimeStamp ? lamportTime : otherProcessTimeStamp) + 1;
    pthread_mutex_unlock(&lamportTimeMutex);
}

int GetLamportTime()
{
    pthread_mutex_lock(&lamportTimeMutex);
    int currentLamportTime = lamportTime;
    pthread_mutex_unlock(&lamportTimeMutex);
    return currentLamportTime;
}

std::string toString(int number)
{
    std::ostringstream stm;
    stm << number;
    return stm.str();
}