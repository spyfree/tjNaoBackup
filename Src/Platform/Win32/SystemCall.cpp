#include "SystemCall.h"
#include "windows.h"

unsigned SystemCall::getCurrentSystemTime()
{
	return getRealSystemTime();
}

unsigned SystemCall::getRealSystemTime()
{
	return timeGetTime();
}

void SystemCall::sleep(unsigned int ms)
{
	Sleep(ms);
}