#ifndef __RobotStateProvider_h_
#define __RobotStateProvider_h_
#include "Module/Representations//KS.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/RobotState.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/BaseProvider.h"
#include "SensorHelp/GroundContactDetector.h"
#include "SensorHelp/CoPDetector.h"
DEFINE_KS(RobotStateProvider)
REQUIRES(NaoStructure)
REQUIRES(SensoryJointData)
REQUIRES(SafeSensorData)
REQUIRES(SensorData)
REQUIRES(ActuatorFrameInfo)
PROVIDES(RobotState)
END_KS

class RobotStateProvider : public RobotStateProviderBase,public BaseProvider
{
public:
	RobotStateProvider();
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void inSensoryJointData(){}
	void update(RobotState* theRobotState);
	void inNaoStructure(){}
	void inSafeSensorData(){}
	void inSensorData(){}
	void inActuatorFrameInfo(){}
	void outRobotState(){}

private:
	bool receiveExternRobotState(RobotState* theRobotState);
	RobotState externRobotState;
	GroundContactDetector groundContactDetector;
	CoPDetector copDetector;
};
#endif