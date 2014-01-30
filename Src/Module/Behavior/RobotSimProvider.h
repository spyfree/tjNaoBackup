#ifndef RobotSimProvider_H
#define RobotSimProvider_H
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Infrastructure/BehaviorData.h"
#include "Module/Representations/Infrastructure/OutBehaviorData.h"
#include "Module/Common/JointData.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "RunSimBehav.h"

DEFINE_KS(BehaviorSimProvider)
REQUIRES(GoalPercept)
REQUIRES(BallPercept)
REQUIRES(RobotPercept)
REQUIRES(RobotPose)
REQUIRES(BallModel)
REQUIRES(FreePartOfOpponentGoalModel)
//REQUIRES(TeamMateInfo)
REQUIRES(OutBehaviorData)
REQUIRES(RoboCupGameControlData)
PROVIDES(MotionCmd)
USES(MotionCmd)
PROVIDES(BehaviorData)
PROVIDES(RoboCupGameControlData)
PROVIDES(TeamCfgInfo)
//USES(TeamCfgInfo)
PROVIDES(RobotInfo)
END_KS

class BehaviorSimProvider:public BehaviorSimProviderBase,public BaseProvider
{
public:
	BehaviorSimProvider();
	~BehaviorSimProvider();
private:
	void update(BehaviorData* theBehaviorData);
	void outBehaviorData();
	void inGoalPercept(){}
	void inBallPercept(){}
	void inRobotPercept(){}
	void inRobotPose(){}
    void inBallModel(){}
	void inFreePartOfOpponentGoalModel(){}
	//void inTeamMateInfo();
	void inOutBehaviorData();
	void inRoboCupGameControlData();
	//----------Send To motion-----//
	void update(MotionCmd* theMotionCmd){}
	void outMotionCmd(){}
	void update(RoboCupGameControlData* theRoboCupGameControlData);
	void outRoboCupGameControlData(){}
	void update(TeamCfgInfo* theTeamCfgInfo);
	void outTeamCfgInfo(){}
	void update(RobotInfo*theRobotInfo);
	void outRobotInfo(){}
	void runBehavior();
	void readTeamConfig();

private:
	PyRobotSimInterface *thePyRobot;
	TeamCfgInfo *theCurrentTeamCfgData;
	LEDSimRequest*theled;
	//perceiveInfo* theCurrentLocData;
};


class RobotManager
{
public:
	RobotManager();
	~RobotManager();
	void initRobotKs();
	void executeRobKs();
	void destoryRobKs();
private:
	void setGlobal();
	// ---- new a self KSManager
	void initKSManager();
	void updateKSManager();
	Blackboard *blackboard;
	KSManager *theKSManager;


};
#endif
