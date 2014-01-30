#include "Blackboard.h"
#include <string.h>
#include <stdlib.h>

Blackboard::Blackboard()
:
theJointCalibration(NULL),
theSensorCalibration(NULL),
theRobotDimensions(NULL),
theMassCalibration(NULL),
theHardnessSettings(NULL),

theRobotModel(NULL),
theTorsoMatrix(NULL),

theNaoConfig(NULL),
theCameraConfig(NULL),
theSensoryJointData(NULL),
theMotionJointData(NULL),
theNaoStructure(NULL),
theFallDownState(NULL),
theCameraMatrix(NULL),
//theCameraTopMatrix(NULL),
//theCameraBottomMatrix(NULL),
//Motion
theMotionCmd(NULL),
theMotionPlan(NULL),
theJointCmd(NULL),
theSafeJointData(NULL),
theWalkingEngineOutput(NULL),
theSpecActionOutput(NULL),

theBWalkingEngineOutput(NULL),

theRobotState(NULL),
//theInertiaMatrix(NULL),
theOdometryData(NULL),

//GameControl
theRoboCupGameControlData(NULL),
theTeamMateInfo(NULL),
theTeamUDPData(NULL),
theGlobalInfo(NULL),
theTeamCfgInfo(NULL),
theRobotInfo(NULL),
theTJImage(NULL),
theFrameInfo(NULL),
thenetVisionInterface(NULL),
// theVisionInterface(NULL),
theBehaviorData(NULL),
theOutBehaviorData(NULL),
theNetworkData(NULL),
theWalkParameters(NULL),
theBallSpots(NULL),
theBallPercept(NULL),
theLinesPercept(NULL),
theGoalPercept(NULL),
theRegionpercept(NULL),
theVisionSimInterface(NULL),
theBlobPercept(NULL),
theRobotPercept(NULL),

theSensorData(NULL),
theSafeSensorData(NULL),

theKeyStates(NULL),
//localization
theBallModel(NULL),
theRobotPose(NULL),
theFieldDimensions(NULL),
thePotentialRobotPose(NULL),
theFreePartOfOpponentGoalModel(NULL),

theActuatorFrameInfo(NULL),

theLEDRequest(NULL),
theFreePartData(NULL),
theRobotsamplebuffer(NULL),
theBallsamplebuffer(NULL),
theForSampleToPc(NULL)
{
}

void Blackboard::operator=(const Blackboard& other)
{
	memcpy(this, &other, sizeof(Blackboard));
}

void* Blackboard::operator new(unsigned size)
{
	return calloc(1, size);
}

void Blackboard::operator delete(void* p)
{
	return free(p);
}
void Blackboard::distract()
{
}

PROCESS_WIDE_STORAGE Blackboard* Blackboard::theInstance = NULL;
