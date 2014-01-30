#ifndef __Blackboard_h_
#define __Blackboard_h_

#include "Platform/SystemCall.h"
//add the data here if you want add more data on blackboard
class JointCalibration;
class SensorCalibration;
class RobotDimensions;
class MassCalibration;
class HardnessSettings;

class RobotModel;
class TorsoMatrix;
//class OdometryData;


class NaoConfig;
class CameraConfig;
//Image
class TJImage;
class FrameInfo;
class netVisionInterface;
// class VisionInterface;
//Robot
class BehaviorData;
class WalkingParameters;
class OutBehaviorData;
//motion
class MotionCmd;
class MotionPlan;
class JointCmd;
class SafeJointData;
class WalkingEngineOutput;
class SpecActionOutput;

class BWalkingEngineOutput;

class RobotState;
//class InertiaMatrix;
class OdometryData;

//networkd
class NetworkData;
//friends
class Process;
//Sensor
class SensoryJointData;
class MotionJointData;
class NaoStructure;
class FallDownState;
class SensorData;
class SafeSensorData;
class KeyStates;
//vision
//class CameraTopMatrix;
//class CameraMatrix;
class CameraMatrix;
class BallSpots;
class BallPercept;
class LinesPercept;
class GoalPercept;
class Regionpercept;
class VisionSimInterface;
class BlobPercept;//for robot detection
class RobotPercept;

//network
class RoboCupGameControlData;
class TeamMateInfo;
class TeamUDPData;
class GlobalInfo;
class TeamCfgInfo;

class RobotInfo;

//Locator
class BallModel;
class RobotPose;
class FieldDimensions;
class PotentialRobotPose;
class FreePartOfOpponentGoalModel;

class ActuatorFrameInfo;

class LEDRequest;
//for noline locator tool
class FreePartData;
class  Robotsamplebuffer;
class Ballsamplebuffer;
class ForSampleToPc;

class Blackboard
{
protected:
	const JointCalibration* theJointCalibration;
	const SensorCalibration* theSensorCalibration;
	const RobotDimensions* theRobotDimensions;
	const MassCalibration* theMassCalibration;
	const HardnessSettings* theHardnessSettings;

	const RobotModel* theRobotModel;
	const TorsoMatrix* theTorsoMatrix;

	//configuration
	const NaoConfig* theNaoConfig;
	const CameraConfig* theCameraConfig;
	//sensor
	const SensoryJointData* theSensoryJointData;
	const MotionJointData*theMotionJointData;
	const NaoStructure* theNaoStructure;
	const FallDownState* theFallDownState;
	//Cognition
     CameraMatrix* theCameraMatrix;//whether is const?
	//CameraTopMatrix* theCameraTopMatrix;
	//CameraMatrix* theCameraBottomMatrix;
	//Motion

	MotionCmd* theMotionCmd; 
	const MotionPlan* theMotionPlan;
	const JointCmd* theJointCmd;
	const SafeJointData* theSafeJointData;
	const WalkingEngineOutput* theWalkingEngineOutput;
	const SpecActionOutput* theSpecActionOutput;

	const BWalkingEngineOutput* theBWalkingEngineOutput;

	const RobotState* theRobotState;
	//const InertiaMatrix* theInertiaMatrix;
#ifdef NAO_DEBUG
	OdometryData* theOdometryData;
	FrameInfo* theFrameInfo;
	BallPercept*theBallPercept;
	LinesPercept* theLinesPercept;
#else
	OdometryData* theOdometryData;
	const FrameInfo* theFrameInfo;
	const BallPercept*theBallPercept;
	const LinesPercept*theLinesPercept;
#endif

	const TJImage* theTJImage;
	const netVisionInterface* thenetVisionInterface;
	//const VisionInterface* theVisionInterface;
	const BehaviorData* theBehaviorData;
	OutBehaviorData* theOutBehaviorData;

	const NetworkData*theNetworkData;
	RoboCupGameControlData*theRoboCupGameControlData;
	TeamMateInfo* theTeamMateInfo;
	TeamUDPData* theTeamUDPData;
	GlobalInfo* theGlobalInfo;
	TeamCfgInfo* theTeamCfgInfo;
	RobotInfo* theRobotInfo;
	const WalkingParameters* theWalkParameters;
	const BallSpots* theBallSpots;
	const GoalPercept*theGoalPercept;
     Regionpercept*theRegionpercept;
	const VisionSimInterface* theVisionSimInterface;
    const BlobPercept*theBlobPercept;
	const RobotPercept*theRobotPercept;

	const SensorData *theSensorData;
	const SafeSensorData* theSafeSensorData;
	const KeyStates *theKeyStates;

     //localization
	const BallModel *theBallModel;
	const RobotPose *theRobotPose;
	const FieldDimensions *theFieldDimensions;
	const PotentialRobotPose* thePotentialRobotPose;
    const FreePartOfOpponentGoalModel* theFreePartOfOpponentGoalModel;
	const ActuatorFrameInfo *theActuatorFrameInfo;

	const LEDRequest *theLEDRequest;

	const FreePartData * theFreePartData;
	const Robotsamplebuffer*theRobotsamplebuffer;
	const Ballsamplebuffer*theBallsamplebuffer;
	const ForSampleToPc* theForSampleToPc;//只是为了产生一个KS用于将球和人定位的粒子传回到Pc机上，没有任何实际意义。

	PROCESS_WIDE_STORAGE_STATIC Blackboard* theInstance;

	/**
	* The method is a dummy that is called to prevent the compiler from certain
	* optimizations in a method generated in Module.h.
	* It is empty, but important, not defined inline.
	*/
	static void distract();
private:
	/**
	* Default constructor.
	*/
	Blackboard();

public:
	/**
	* Virtual destructor.
	* Required for derivations of this class.
	*/
	virtual ~Blackboard() {}

	/**
	* Assignment operator.
	* Note: copies will share all representations.
	* @param other The instance that is cloned.
	*/
	void operator=(const Blackboard& other);
	/**
	* The operator allocates a memory block that is zeroed.
	* Therefore, all members of this class are initialized with 0.
	* @attention This operator is only called if this class is instantiated by
	* a separate call to new, i.e. it cannot be created as a part of another class.
	* @param size The size of the block in bytes.
	* @return A pointer to the block.
	*/
	static void* operator new(unsigned size);

	/**
	* The operator frees a memory block.
	* @param p The address of the block to free.
	*/
	static void operator delete(void* p);

	friend class Actuator;
	friend class NetworkManager;
	friend class RobotManager;
};

#endif
