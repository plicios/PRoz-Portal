#ifndef GLOBALS_GUARD
#define GLOBALS_GUARD

#include <string>

#define PortalGrantedTag 1
#define OtherTag 2

#define UP "up"
#define DOWN "down"

struct PortalPlace{
    int processId;
    int timeStamp;
    std::string direction;
};

enum MessageType{
    PortalDownRequest = 0,
    PortalUpRequest = 1,
    PortalRelease = 2,
    PortalAgree = 3
};

struct Message{
    int messageType;
    int processId;
    int timeStamp;
};
#endif