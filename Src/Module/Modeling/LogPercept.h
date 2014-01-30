#ifndef LogPercept_H
#define LogPercept_H

/**
*该类通过离线方式将定位所需的数据通过KS形式传给定位线程
*/

#include "Module/Network/RoboCupGameControlData.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/KS.h"
#include "Module/Modeling/ReadLogData.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include <fstream>

DEFINE_KS(LogPercept)
PROVIDES(GoalPercept)
PROVIDES(LinesPercept)
PROVIDES(OdometryData)
PROVIDES(RoboCupGameControlData)
PROVIDES(RobotInfo)
//PROVIDES(TeamCfgInfo)
PROVIDES(FrameInfo)
PROVIDES(FieldDimensions)
PROVIDES(CameraMatrix)
PROVIDES(BallPercept)
PROVIDES(MotionCmd)
USES(OdometryData)
USES(FrameInfo)
USES(CameraMatrix)
USES(BallPercept)
USES(MotionCmd)
USES(LinesPercept)
END_KS

class LogPercept : public LogPerceptBase
{
public:
	LogPercept();
	~LogPercept();
	void outGoalPercept(){}
	void update(GoalPercept* theGoalPercept);
	void outLinesPercept(){}
	void update(LinesPercept* theLinesPercept){}
	void outOdometryData(){}
	void update(OdometryData* theOdometryData){}
	void outRoboCupGameControlData(){}
	void update(RoboCupGameControlData* theRoboCupGameControlData);
	void outRobotInfo(){}
	void update(RobotInfo* theRobotInfo);
	//void outTeamCfgInfo(){}
	//void update(TeamCfgInfo* theTeamCfgInfo);
	void outFrameInfo(){}
	void update(FrameInfo* theFrameInfo){}
	void outFieldDimensions(){}
	void update(FieldDimensions* theFieldDimensions){}
	void outCameraMatrix(){}
	void update(CameraMatrix* theCameraBottomMatrix){}
	void outBallPercept(){}
	void update(BallPercept* theBallPercept){}
	void outMotionCmd(){}
	void update(MotionCmd* theMotionCmd){}

};

#endif