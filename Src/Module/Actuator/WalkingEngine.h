/**
* @file Module/Motion/WalkingEngine.h
* This Engine for walking control.
* @author XU Tao
*/
#ifndef __WalkingEngine_h_
#define __WalkingEngine_h_
#include "Module/Representations//KS.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Common/RobotState.h"
#include "WalkTypeBase.h"
#include "PreviewWalk2010/PreviewWalk.h"

DEFINE_KS(WalkingEngine)
REQUIRES(MotionPlan)
REQUIRES(NaoStructure)
REQUIRES(SensoryJointData)
REQUIRES(SafeJointData)
REQUIRES(SafeSensorData)
REQUIRES(RobotState)
PROVIDES(WalkingEngineOutput)
END_KS

class WalkingEngine : public WalkingEngineBase
{
public:
	WalkingEngine();
	~WalkingEngine();
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void inSafeJointData(){}
	void inRobotState(){ robotState = *theRobotState;}
	void inSensoryJointData(){}
	void update(WalkingEngineOutput* theWalkingEngineOutput);
	void inMotionPlan();
	void inNaoStructure();
	void inSafeSensorData();
	void outWalkingEngineOutput();
private:
	bool checkCmdUpdate(const WalkCmd& walkCmd);
	void presetWalkOutput(WalkingEngineOutput* theWalkingEngineOutput);
	void processSensorData(const SafeSensorData* theSafeSensorData);
private:
	WalkTypeBase* pWalker;
	PreviewWalk* previewWalker;

	WalkCmd lastWalkCmd;
	WalkingEngineOutput lastWalkOutput;
	WalkingEngineOutput standingOutput;
	int lastMotionID;
	SafeSensorData processedSensorData;
	RobotState robotState;
};
#endif