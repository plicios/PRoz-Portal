#include "mainProgram.h"
#include "common.h"
#include "utils.h"
#include "portal.h"
#include "globals.h"
#include "communicator.h"

void RequestPortal(bool requestUp)
{
    ResetAgrees();
    setWaitingForPortal(true);

    Message message;
    message.processId = getProcessId();
    message.timeStamp = GetLamportTime();
    message.messageType = requestUp ? PortalUpRequest : PortalDownRequest;

    UpdateLamportClock(0);

    SetMyPortalRequestDirection(requestUp ? UP : DOWN);
    AddToPortal(message.processId, message.timeStamp, requestUp ? UP : DOWN);
    SendToAll(message, OtherTag);
}

void ReleasePortal()
{
    UpdateLamportClock(0);

    Message message;
    message.timeStamp = GetLamportTime();
    message.processId = getProcessId();
    message.messageType = PortalRelease;

    RemoveFromPortal(message.processId);
    SendToAll(message, OtherTag);
}

void WaitForPortal()
{
    PortalGrantedGet();
}

void WorkDown()
{
    ShowMessage(true, "Start working down");
    Wait(10000,20000);
    ShowMessage(true, "Stop working down");
}

void WorkUp()
{
    ShowMessage(true, "Start working up");
    Wait(10000,20000);
    ShowMessage(true, "Stop working up");
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
