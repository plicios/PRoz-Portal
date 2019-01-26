#include <sstream>
#include <vector>
#include "common.h"
#include "communicator.h"
#include "utils.h"
#include "portal.h"

using namespace std;

int* processesTimeStamps;
std::vector<int> responsePortalQueue = std::vector<int>();
bool shouldHandleMessage(Message message);
void SendPortalResponsesToQueue();

void *receiver(void*)
{
	processesTimeStamps = new int[getProcessNumber()];
	for (int i = 0; i < getProcessNumber(); i++)
	{
		processesTimeStamps[i] = -1;
	}
    while(getExecuteProgram())
    {
//        ShowMessage(false, "start loop");
        Message message = Receive(OtherTag, false);

//		if(shouldHandleMessage(message))
		if(true)
		{
			ostringstream stringStream;
			stringStream << "got message: " << message.processId<< " : " << message.timeStamp << " : " << message.messageType << " : " << message.data << endl;
			ShowMessage(false, stringStream.str());

			switch(message.messageType)
			{
				case PortalRequest:
                {
					int myRequestTimestamp = GetMyRequestTimestamp();

					if(myRequestTimestamp == -1 || myRequestTimestamp > message.timeStamp || (myRequestTimestamp == message.timeStamp && getProcessId() > message.processId))
					{
						Message responseMessage;
						responseMessage.processId = getProcessId();
						responseMessage.timeStamp = GetLamportTime();
						responseMessage.messageType = PortalResponse;
						responseMessage.data = GetInPortal();

						Send(message.processId, responseMessage, OtherTag);

						ShowMessage(false, "Agree sent");
					}
					else
					{
						responsePortalQueue.push_back(message.processId);
						ShowMessage(false, "Added to queue");
					}
                    break;
                }
                case PortalRelease:
                {
					if(message.data != 0)
					{
						UpdatePortalStatus(-message.data);
					}
                    break;
                }
                case PortalResponse:
                {
					IncrementResponseCounter();
					if(message.data != 0)
					{
						UpdatePortalStatus(message.data);
					}
                    break;
                }
			}

			if(GetResponseCounter() == getProcessNumber() - 1 && (message.messageType == PortalResponse || message.messageType == PortalRelease))
			{
				int portalStatus = GetPortalStatus();
				int peopleInPortal = portalStatus > 0 ? portalStatus : -portalStatus;
				int portalDirection = 0;
				if(portalStatus > 0)
				{
					portalDirection = 1;
				}
				else if(portalStatus < 0)
				{
					portalDirection = -1;
				}

				if((GetMyDirection() == portalDirection || portalDirection == 0) && peopleInPortal < getPortalCapacity())
				{
					SetMyRequestTimestamp(-1);

					SetInPortal(GetMyDirection());
					PortalGrantedSend();

					SendPortalResponsesToQueue();
				}
			}
		}
    }
	delete(processesTimeStamps);
}

void SendPortalResponsesToQueue()
{
	for(int i = 0; i < responsePortalQueue.size(); i++)
	{
		Message responseMessage;
		responseMessage.processId = getProcessId();
		responseMessage.timeStamp = GetLamportTime();
		responseMessage.messageType = PortalResponse;
		responseMessage.data = GetInPortal();

		Send(responsePortalQueue[i], responseMessage, OtherTag);
		ShowMessage(false, "Response sent");
	}

	responsePortalQueue.clear();
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
