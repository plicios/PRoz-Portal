#include <sstream>
#include "common.h"
#include "communicator.h"
#include "utils.h"
#include "portal.h"

using namespace std;

int* processesTimeStamps;

bool shouldHandleMessage(Message message);

void *receiver(void*)
{
	processesTimeStamps = new int[getProcessNumber()];
	for (int i = 0; i < getProcessNumber(); i++)
	{
		processesTimeStamps[i] = -1;
	}
    while(getExecuteProgram())
    {
        ShowMessage(false, "start loop");
        Message message = Receive(OtherTag, false);

		if(shouldHandleMessage(message))
		{
			ostringstream stringStream;
			stringStream << "got message: " << message.processId<< " : " << message.timeStamp << " : " << message.messageType << endl;
			ShowMessage(false, stringStream.str());

			switch(message.messageType)
			{
				case PortalDownRequest:
                {
                    AddToPortal(message.processId, message.timeStamp, DOWN);

					Message agreeMessage;
					agreeMessage.processId = getProcessId();
					agreeMessage.timeStamp = GetLamportTime();
					agreeMessage.messageType = PortalAgree;

					Send(message.processId, agreeMessage, OtherTag);
					ShowMessage(false, "Agree sent");
                    break;
                }
                case PortalUpRequest:
                {
					AddToPortal(message.processId, message.timeStamp, UP);

					Message agreeMessage;
					agreeMessage.processId = getProcessId();
					agreeMessage.timeStamp = GetLamportTime();
					agreeMessage.messageType = PortalAgree;

					Send(message.processId, agreeMessage, OtherTag);
					ShowMessage(false, "Agree sent");
                    break;
                }
                case PortalRelease:
                {
					RemoveFromPortal(message.processId);
                    break;
                }
                case PortalAgree:
                {
                    IncrementAgrees();
                    break;
                }
			}

			if((message.messageType == PortalAgree || message.messageType == PortalRelease) )
			{
				ostringstream stringStream2;
				stringStream2 << "got " << GetAgrees() << " agrees" << endl;
				ShowMessage(false, stringStream2.str());

				if(GetAgrees() >= getProcessNumber())
				{
					ostringstream stringStream3;
					stringStream3 << "Portal capacity: " << getPortalCapacity() << endl;
					stringStream3 << "Portal direction: " << GetPortalDirection() << endl;
					stringStream3 << "This many before me: " << GetPeopleBeforeMe() << endl;
					ShowMessage(false, stringStream3.str());

					if (getPortalCapacity() - GetPeopleBeforeMe() > 0 && GetPortalDirection() == GetMyPortalRequestDirection())
					{
						if(getWaitingForPortal())
						{
							ShowMessage(false, "Let main program do portal work");
							setWaitingForPortal(false);
							PortalGrantedSend();
						}
						else
						{
							ShowMessage(false, "Not waiting.");
						}
					}
					else
					{
						ShowMessage(false, "Not enough portal places or wrong portal direction");
					}
				}
			}
		}
    }
	delete(processesTimeStamps);
}

bool shouldHandleMessage(Message message)
{
	if(message.timeStamp > processesTimeStamps[message.processId])
	{
		processesTimeStamps[message.processId] = message.timeStamp;
		return true;
	}
	else
	{
		return false;
	}
}
