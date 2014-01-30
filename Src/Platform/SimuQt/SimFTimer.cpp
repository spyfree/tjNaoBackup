#include "SimFTimer.h"
SimFTimer* SimFTimer::theSimFTimer = 0;
SimFTimer::SimFTimer()
{
	resetAllFTimer();
}

void SimFTimer::setFTimer(int id, int time)
{
	if(id < NumTimer){
			timers[id].setTime(time);
	}
}
bool SimFTimer::checkFTimer(int id)
{
	if(id < NumTimer && timers[id].active)
	{
		return timers[id].counter<= 0;
	}else
	{
		return false;
	}
}
void SimFTimer::resetFTimer(int id)
{
	if(id < NumTimer)
		timers[id].reset();
}
void SimFTimer::resetAllFTimer()
{
	for(int i = 0; i < NumTimer; ++i)
		resetFTimer(i);
}
void SimFTimer::counteDown()
{
	for(int i = 0; i < NumTimer; ++i)
	{
		if(timers[i].active && timers[i].counter > 0)
		{
			if(--(timers[i].counter)<=0 )
				timers[i].timeup = true;
		}
	}
}
void SimFTimer::Init()
{
	if(!theSimFTimer)
	{
		theSimFTimer = new SimFTimer();
	}
}
SimFTimer* SimFTimer::getInstance()
{
	if(!theSimFTimer)
		Init();
	return theSimFTimer;
}