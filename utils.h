#include <string>

void Wait(int minWaitTime, int maxWaitTime);
void ShowMessage(bool isMain, std::string message);
void UpdateLamportClock(int otherProcessTimeStamp);
int GetLamportTime();
std::string toString(int number);