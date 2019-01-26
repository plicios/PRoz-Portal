#include "mainProgram.h"
#include "common.h"
#include "utils.h"
#include "portal.h"
#include "globals.h"
#include "communicator.h"



void RequestPortal(bool requestUp)
{
    ClearPortalStatus();

    Message message;
    message.processId = getProcessId();
    message.timeStamp = GetLamportTime();
    message.messageType = PortalRequest;
    message.data = requestUp ? 1 : -1;

    UpdateLamportClock(0);

    SetMyRequestTimestamp(GetLamportTime());

    SendToAll(message, OtherTag);
}

void ReleasePortal()
{
    Message message;
    message.timeStamp = GetLamportTime();
    message.processId = getProcessId();
    message.messageType = PortalRelease;
    message.data = GetInPortal();

    ChangeLocation();
    SetInPortal(0);
    UpdateLamportClock(0);

    SendToAll(message, OtherTag);
}

void WaitForPortal()
{
    PortalGrantedGet(GetMyRequestTimestamp());
}

void WorkDown()
{
    ShowMessage(true, "Start working down");
    Wait(10000,20000);
    ShowMessage(true, "Stop working down");
    UpdateLamportClock(0);
}

void WorkUp()
{
    ShowMessage(true, "Start working up");
    Wait(10000,20000);
    ShowMessage(true, "Stop working up");
    UpdateLamportClock(0);
}

void GoUp()
{
    ShowMessage(true, "Start request portal up");
    RequestPortal(true);
    WaitForPortal();
    ShowMessage(true, "Portal up granted");
    ShowMessage(true, "Going up");
    Wait(10000,20000);
    ReleasePortal();
    ShowMessage(true, "Is up");
}

void GoDown()
{
    ShowMessage(true, "Start request portal down");
    RequestPortal(false);
    WaitForPortal();
    ShowMessage(true, "Portal down granted");
    ShowMessage(true, "Going down");
    Wait(10000,20000);
    ReleasePortal();
    ShowMessage(true, "Is down");
}

void mainProgram()
{
    while(getExecuteProgram())
    {
        WorkDown();
        GoUp();
        WorkUp();
        GoDown();
    }
}
