#ifndef PYCOMMON_H
#define PYCOMMON_H
#include "Module/Representations/Infrastructure/OutBehaviorData.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;

class CommonModule
{
public:
	CommonModule();
	~CommonModule();
	int getFallState();
	int getBumperRight();
	int getBumperLeft();
	int getChestButton();
	float getLeftFootX();
	float getLeftFootY();
	float getRightFootX();
	float getRightFootY();

	float getHeadYaw();
	float getHeadPitch(); //2
	float getRShoulderPitch();
	float getRShoulderRoll();
	float getRElbowYaw();
	float getRElbowRoll();//6
	float getLShoulderPitch();
	float getLShoulderRoll();
	float getLElbowYaw();
	float getLElbowRoll();//10
	float getRHipYawPitch();
	float getRHipRoll();
	float getRHipPitch();
	float getRKneePitch();//14
	float getRAnklePitch();
	float getRAnkleRoll();
	float getLHipYawPitch();
	float getLHipRoll();//18
	float getLHipPitch();
	float getLKneePitch();
	float getLAnklePitch();
	float getLAnkleRoll();//22

private:
	const OutBehaviorData * outBehaviorData;
};

void set_common_reference(const OutBehaviorData * outBehaviorData);
void c_init_common();


#endif
