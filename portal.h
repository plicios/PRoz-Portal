#include <string>

void IncrementAgrees();
void ResetAgrees();
int GetAgrees();

void AddToPortal(int processId, int timeStamp, std::string direction);
void RemoveFromPortal(int processId);
int GetMyPortalRequestTimeStamp();
int GetPeopleBeforeMe();

void SetMyPortalRequestDirection(std::string _myDirection);
std::string GetMyPortalRequestDirection();
std::string GetPortalDirection();