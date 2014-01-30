#include "WalkingEngine.h"
#include <iostream>
WalkingEngine::WalkingEngine()
	:pWalker(NULL), previewWalker(NULL)
{
	previewWalker = new PreviewWalk();
	pWalker = previewWalker;
	lastMotionID = theMotionPlan->motion != MotionCmd::Stand;
	double jdataDeg[] = {0, 0,
		100, -30, 90, 90,
		100, 30, -90, -90,
		0, 0, -40, 80, -40, 0,
		0, 0, -40, 80, -40, 0};
	for(int i = JointData::NoneJoint + 1; i < JointData::numOfJoints; ++i)
	{
		standingOutput.angles[i] = deg2rad(jdataDeg[i-1]) ;
	}
}
WalkingEngine::~WalkingEngine()
{
	if(previewWalker){
		delete previewWalker;
		previewWalker = NULL;
	}
	pWalker = NULL;
}
void WalkingEngine::update(WalkingEngineOutput* theWalkingEngineOutput)
{
	//If not walk, it's none of my business.
	if(theMotionPlan->motion != MotionCmd::Walk && theMotionPlan->motion != MotionCmd::Stand){
		lastMotionID = theMotionPlan->motion;
		return;
	}
	//update cmd
	checkCmdUpdate(theMotionPlan->walkCmd);
	//==
	presetWalkOutput(theWalkingEngineOutput);
	//==execute walker
	inSafeSensorData();
	inRobotState();
	pWalker->setSensorData(&processedSensorData);
	pWalker->setRobotState(&robotState);
	pWalker->goNextFrame(lastWalkOutput, *theWalkingEngineOutput, *theNaoStructure);
	//if(pWalker->getWalkState() == WalkTypeBase::standing)
	//{
		//*theWalkingEngineOutput = standingOutput;
	//}
	//lastWalkOutput = *theWalkingEngineOutput;
	//modify supporting mode....Maybe we can get a much better way...
	const_cast<NaoStructure*>(theNaoStructure)->supportingMode = pWalker->getSupMode();
	//===
	lastMotionID = theMotionPlan->motion;

}
void WalkingEngine::inMotionPlan()
{

}
void WalkingEngine::inNaoStructure()
{

}
void WalkingEngine::outWalkingEngineOutput()
{

}

bool WalkingEngine::checkCmdUpdate(const WalkCmd& walkCmd)
{
	bool cmdChanged = false;
	if(walkCmd.walkType != lastWalkCmd.walkType)
	{//Walk type changed
		lastWalkCmd = walkCmd;
		switch(theMotionPlan->walkCmd.walkType)
		{
		case WalkCmd::eCartTableWalk:
			pWalker = previewWalker;
			pWalker->updateWalkCmd(walkCmd);
			break;
		default:
			break;
		}
		cmdChanged  = true;
	}else if(walkCmd.speed != lastWalkCmd.speed)
	{
		lastWalkCmd = walkCmd;
		pWalker->updateWalkCmd(walkCmd);
		cmdChanged = true;
	}
	if(cmdChanged){
            std::cout<<"Detect Walk Cmd Change: "<< walkCmd.walkType<<" Speed: "<<
            walkCmd.speed.translation.x<<", "<<walkCmd.speed .translation.y<<", "<<walkCmd.speed .rotation<<std::endl;
    }

	return cmdChanged;
}
void WalkingEngine::presetWalkOutput(WalkingEngineOutput* theWalkingEngineOutput)
{
	for(int i = JointData::HeadYaw; i < JointData::numOfJoints; ++i)
	{
		if(lastWalkOutput.angles[i] == JointData::off || lastWalkOutput.angles[i] == JointData::ignore)
		{
			theWalkingEngineOutput->angles[i] = theSafeJointData->angles[i];
			lastWalkOutput.angles[i] = theSafeJointData->angles[i];
		}else
		{
			if(lastMotionID == MotionCmd::Walk || lastMotionID == MotionCmd::Stand)
				theWalkingEngineOutput->angles[i] = lastWalkOutput.angles[i];//maybe sensory joint data ?
			else{
				theWalkingEngineOutput->angles[i] = theSafeJointData->angles[i];//lastWalkOutput.angles[i];//maybe sensory joint data ?
				lastWalkOutput.angles[i] = theWalkingEngineOutput->angles[i];
			}
		}
	}
}
void WalkingEngine::inSafeSensorData()
{
	processSensorData(theSafeSensorData);
}
void WalkingEngine::processSensorData(const SafeSensorData* theSafeSensorData)
{
	processedSensorData = *theSafeSensorData;
	//====Calibration Sensor Data, this should be placed just after sensor read!====//
// #ifndef NAO_DEBUG//on real robot
// 	double gAcc = 59;
// #else
// 	double gAcc = 1;
// #endif
// 	processedSensorData.data[SensorData::accX] *= 9.8 / gAcc;
// 	processedSensorData.data[SensorData::accY] *= 9.8 / gAcc;
// 	processedSensorData.data[SensorData::accZ] *= 9.8 / gAcc;
}

MAKE_KS(WalkingEngine)
