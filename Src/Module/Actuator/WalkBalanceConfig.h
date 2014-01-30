#ifndef __WalkBalanceConfig_h_
#define __WalkBalanceConfig_h_

class WalkBalanceConfig
{
public:
	WalkBalanceConfig();
	~WalkBalanceConfig();
private:
	void defaultInit();

public:
	float angleXmax;
	float angleXmin;
	float angleYmax;
	float angleYmin;

	int Ulimt;
	int Dlimt;
	int UnableLimt;
	int NumSonar;

	float newAngleXmax;
	float newAngleXmin;
	float newAngleYmax;
	float newAngleYmin;

	float detX;
	float detY;
};

#endif