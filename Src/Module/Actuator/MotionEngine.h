/**
* @file Module/Motion/WalkingEngine.h
* This Engine for Motion control.
* @author XU Tao
*/
#ifndef __MotionEngine_h_
#define __MotionEngine_h_
#include "Module/Representations/KS.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Common/JointData.h"
#include "Module/Common/RobotState.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Common/SpecActionOutput.h"

DEFINE_KS(MotionEngine)
REQUIRES(MotionCmd)
REQUIRES(SafeJointData)
REQUIRES(WalkingEngineOutput)
REQUIRES(SpecActionOutput)
REQUIRES(RobotState)
PROVIDES(MotionPlan)
PROVIDES(JointCmd)
END_KS

class MotionEngine : public MotionEngineBase
{
public:
	MotionEngine();
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void update(MotionPlan* theMotionPlan);
	void update(JointCmd* theJointCmd);
	void inMotionCmd();
	void inWalkingEngineOutput(){}
	void inSpecActionOutput(){}
	void inSafeJointData(){}
	void inRobotState(){}
	void outMotionPlan();
	void outJointCmd(){}
private:
	/**
	* The method executes the head motion command.
	* @param jointCmd The joint command the two angles of which are set.
	*/
	void headMotion(JointCmd& jointCmd) const;
private:
	MotionPlan currentMotionPlan;
	MotionCmd pendingCmd;
};
#endif