/**
* @file Module/Motion/WalkBalance.h
* This file declares the base class WalkBalance.
*/
#ifndef __WalkBalance_h_
#define __WalkBalance_h_

#include <iostream>
#include <pair>

class WalkBalance
{
public:
	WalkBalance() ;
	~WalkBalance();
	void updateAngles();
	void walkBalanceCheck(std::pair<float,float>);

public:
	float angleX;
	float angleY;
private:
	float angleXmax;
	float angleXmin;
	float angleYmax;
	float angleYmin;

	float sleepTime;

};
#endif