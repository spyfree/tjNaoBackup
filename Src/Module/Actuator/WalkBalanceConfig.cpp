#include "WalkBalanceConfig.h"
#include "iostream.h"

WalkBalanceConfig::WalkBalanceConfig():
detX(0.015),detY(0.02)
{
	defaultInit();
}

WalkBalanceConfig::~WalkBalanceConfig(){}


void WalkBalanceConfig::defaultInit()
{
	angleXmax = 0.11;
	angleXmin = -0.09;
	angleYmax = 0.07;
	angleYmin = -0.15;

	Ulimt = 35;
	Dlimt = 30;
	UnableLimt = 29;
	NumSonar = 1;

	newAngleXmax = 0.11 - detX;
	newAngleXmin = -0.09 + detX;
	newAngleYmax = 0.07 - detY;
	newAngleYmin = -0.15 +detY;
}
