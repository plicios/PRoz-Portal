#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD

#include <string>

#define PortalGrantedTag 1
#define OtherTag 2

enum MessageType{
    PortalRequest = 0,
    PortalRelease = 1,
    PortalResponse = 2
};

struct Message{
    int messageType;
    int processId;
    int timeStamp;
    int data;
};
#endif