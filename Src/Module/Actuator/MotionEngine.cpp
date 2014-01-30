#include "MotionEngine.h"
#include <iostream>
MotionEngine::MotionEngine()
{

}
void MotionEngine::update(MotionPlan* theMotionPlan)
{
	inMotionCmd();

	pendingCmd = *theMotionCmd;
	
	if(pendingCmd.motion == MotionCmd::SpecialAction)
	{
		if(currentMotionPlan.motion == MotionCmd::SpecialAction)
		{
			if(theSpecActionOutput->isLeavingPossible )
			{
				theMotionPlan->motion = pendingCmd.motion;
				theMotionPlan->specActionCmd = pendingCmd.specActionCmd;
			}
		}else if(currentMotionPlan.motion == MotionCmd::Stand || currentMotionPlan.motion == MotionCmd::Walk){
			if(!theWalkingEngineOutput->isLeavingPossible)
			{
				theMotionPlan->walkCmd.speed = Pose2D(0,0,0);
			}else{
				theMotionPlan->motion = MotionCmd::SpecialAction;
				theMotionPlan->specActionCmd = pendingCmd.specActionCmd;
			}
		}
	}else if(pendingCmd.motion == MotionCmd::Stand || pendingCmd.motion == MotionCmd::Walk){
		if(currentMotionPlan.motion == MotionCmd::SpecialAction){
			if(theSpecActionOutput->isLeavingPossible){
				theMotionPlan->motion = pendingCmd.motion;
				theMotionPlan->walkCmd = pendingCmd.walkCmd;

			}
		}else if(currentMotionPlan.motion == MotionCmd::Stand || currentMotionPlan.motion == MotionCmd::Walk)
		{
			theMotionPlan->motion = pendingCmd.motion;
			theMotionPlan->walkCmd = pendingCmd.walkCmd;
		}
	}

	theMotionPlan->headCmd = theMotionCmd->headCmd;
	//If fall detected, we stand...
	if((theRobotState->fallState.fall && !(theRobotState->fallState.recoverd))||
		theRobotState->fallState.updateModel){
		theMotionPlan->motion = MotionCmd::Stand;
		theMotionPlan->walkCmd.speed = Pose2D(0,0,0);
// 		theMotionPlan->motion = MotionCmd::SpecialAction;
// 		theMotionPlan->specActionCmd.specAction = (int)SpecActionCmd::StandUpBackNao;
	}
//	*theMotionPlan = *theMotionCmd;
	currentMotionPlan = *theMotionPlan;
	//std::cout<<"----------------------------->Motion Engine Update<--------------------------"<<std::endl;
}

void MotionEngine::update(JointCmd* theJointCmd)
{
	headMotion(*theJointCmd);
	if(currentMotionPlan.motion != MotionCmd::SpecialAction)
	{
		for(int i = JointData::RShoulderPitch; i < JointData::numOfJoints; ++i)
		{
			theJointCmd->angles[i] = theWalkingEngineOutput->angles[i];
		}
	}else{
		for(int i = JointData::HeadYaw; i < JointData::numOfJoints; ++i)
		{
			if(theSpecActionOutput->angles[i] != JointData::ignore)
				theJointCmd->angles[i] = theSpecActionOutput->angles[i];
		}
	}
}
void MotionEngine::inMotionCmd()
{

}
void MotionEngine::outMotionPlan()
{

}
void MotionEngine::headMotion(JointCmd& jointCmd) const
{
	double speed = fabs(currentMotionPlan.headCmd.speed * 0.04),
		lastPan = jointCmd.angles[JointData::HeadYaw] == JointData::off ? theSafeJointData->angles[JointData::HeadYaw] : jointCmd.angles[JointData::HeadYaw],
		lastTilt = jointCmd.angles[JointData::HeadPitch] == JointData::off ? theSafeJointData->angles[JointData::HeadPitch] : jointCmd.angles[JointData::HeadPitch],
		diffPan = currentMotionPlan.headCmd.pan == JointData::off ? 0 : currentMotionPlan.headCmd.pan - lastPan,
		diffTilt = currentMotionPlan.headCmd.tilt == JointData::off ? 0 : currentMotionPlan.headCmd.tilt - lastTilt,
		diffMax = std::max(fabs(diffTilt), fabs(diffPan));

	if(diffMax > speed)
	{
		double factor =  speed / diffMax;
		diffPan *= factor;
		diffTilt *= factor;
	}

	jointCmd.angles[JointData::HeadYaw] = currentMotionPlan.headCmd.pan == JointData::off ? JointData::off : lastPan + diffPan;
	jointCmd.angles[JointData::HeadPitch] = currentMotionPlan.headCmd.tilt == JointData::off ? JointData::off : lastTilt + diffTilt;

}

MAKE_KS(MotionEngine)
