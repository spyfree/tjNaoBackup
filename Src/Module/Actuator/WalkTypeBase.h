/**
* @file Module/Motion/WalkTypeBase.h
* This file declares the base class repretenting the walk type.
* @author XU Tao
*/
#ifndef __WalkTypeBase_h_
#define __WalkTypeBase_h_
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Common/WalkCmd.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/RobotState.h"
class WalkTypeBase
{
public:
	enum WalkStatus{
		deactive,
		starting,
		ending,
		running,
		standing,
		numWalkStates
	};
	WalkTypeBase() : walkStatus(deactive), istep(0), sup_mod(NaoConfig::SUPPORT_MODE_DOUBLE_RIGHT), pSensorData(0), pRobotState(0){}
	virtual void goNextFrame(WalkingEngineOutput& lastWalkOoutput, WalkingEngineOutput& walkingEngineOutput, const NaoStructure& naoStructure) = 0;
	virtual void reset() = 0;
	virtual void updateWalkCmd(const WalkCmd& newCmd) = 0;
	void setSensorData(SafeSensorData* pSafeSensorData)
	{
		pSensorData = pSafeSensorData;
	}
	void setRobotState(RobotState* pstate){ pRobotState = pstate; }
	virtual int getWalkState()
	{
		return walkStatus;
	}
	virtual int getSupMode(){return sup_mod;}
protected:	
	int walkStatus;
	int istep;
	int sup_mod;
	SafeSensorData* pSensorData;
	RobotState* pRobotState;
	//virtual void setwalkparam
	//virtual void restart...
};
#endif