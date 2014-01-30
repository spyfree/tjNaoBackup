#include "WalkBalance.h"
#include <iostream>

WalkBalance::WalkBalance()
:angleX(0),angleY(0),sleepTime(0),angleXmax(0),angleXmin(0),angleYmax(0),angleYmin(){}

WalkBalance::~WalkBalance(){}


void WalkBalance::updateAngles()
{

}

void WalkBalance::walkBalanceCheck()
{
	updateAngles();
	if(angleX > angleXmax || angleX <angleXmin ||angleY >angleYmax ||angleY<angleYmin)
	{

	}

}
