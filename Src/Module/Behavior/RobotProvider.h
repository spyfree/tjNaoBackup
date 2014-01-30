#ifndef RobotProvider_H
#define RobotProvider_H
#include "Module/Common/LEDRequest.h"//wang-
//#include "Module/Common/ClickButton.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Infrastructure/BehaviorData.h"
#include "Module/Common/JointData.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Representations/Infrastructure/OutBehaviorData.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Behavior/RunBehav.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "Module/Network/PyGlobalInfo.h"
DEFINE_KS(BehaviorProvider)
REQUIRES(GoalPercept)
REQUIRES(BallPercept)
REQUIRES(RobotPercept)
REQUIRES(GlobalInfo)
REQUIRES(RobotPose)
REQUIRES(BallModel)
REQUIRES(FreePartOfOpponentGoalModel)
REQUIRES(OdometryData)
//REQUIRES(TeamMateInfo)
REQUIRES(OutBehaviorData)
PROVIDES(MotionCmd)
USES(MotionCmd)
PROVIDES(TeamUDPData)
USES(TeamUDPData)
PROVIDES(CameraConfig)
USES(CameraConfig)
PROVIDES(BehaviorData)
//PROVIDES(LEDRequest)
PROVIDES(RoboCupGameControlData)
USES(RoboCupGameControlData)
PROVIDES(TeamCfgInfo)
PROVIDES(Ballsamplebuffer)
//USES(TeamCfgInfo)
//PROVIDES(GlobalInfo)
//USES(GlobalInfo)
PROVIDES(GlobalInfo)
PROVIDES(RobotInfo)
USES(TeamMateInfo)
END_KS

#define USE_BUTTON

class BehaviorProvider:public BehaviorProviderBase,BaseProvider
{
public:
	BehaviorProvider();
	~BehaviorProvider();
	void update(BehaviorData* theBehaviorData);
	void outBehaviorData();
	void inGoalPercept(){}
	void inBallPercept(){}
	void inRobotPercept(){}
	void inRobotPose(){}
	void inBallModel(){}
	void inFreePartOfOpponentGoalModel(){}
	void inOdometryData(){}
	void inTeamMateInfo();
	void inRoboCupGameControlData();
	void update(RoboCupGameControlData* theRoboCupGameControlData);
	void outRoboCupGameControlData(){}
	void inGlobalInfo();
    void update(GlobalInfo*theGlobalInfo);
	void outGlobalInfo(){}
	void update(TeamCfgInfo* theTeamCfgInfo);
	void outTeamCfgInfo(){}
	void inOutBehaviorData();
	void update(CameraConfig* theCameraConfig){}
	void outCameraConfig(){}
	void processGameData(RoboCupGameControlData* gameData);
	//bool validatePacket(RoboCupGameControlData *);
	void rawSwapTeams(RoboCupGameControlData* data);
	bool dataEqual(RoboCupGameControlData* data, RoboCupGameControlData* previous);
	bool checkHeader(char*header);
	//----------Send To motion-----//
	void update(MotionCmd* theMotionCmd){}
	void outMotionCmd(){}

	void update(TeamUDPData* theTeamUDPData);
	void outTeamUDPData(){}

	void update(Ballsamplebuffer *theBallsamplebuffer);
	void outBallsamplebuffer(){};
	//----------led indicators-------//
	void indicators(); //-----old ledRequest
	//robot info
	void update(RobotInfo*theRobotInfo);
	void outRobotInfo(){}
	//void update(LEDRequest* theLEDRequest);
	//void outLEDRequest(){}
	void runBehavior();
private:
	void initGameControl();
	void manualGameControl(RoboCupGameControlData* gameData);
	void GCIndicators(RoboCupGameControlData* gameData);
	void readTeamConfig();

#ifdef USE_BUTTON
private:
	void processChestButtonClicks();
	bool executeChestClicksAction(int nClicks);
#endif

private:
	PyRobotInterface *thePyRobot;
	RoboCupGameControlData *theCurrentRobocupGameData;
	TeamMateInfo *theCurrentMateInfo;//data from udp
	GlobalInfo*theCurrentGlobalInfo;


//player number in the robot starts from 1,while the player สýื้ starts from 0, so when be used, player number should -1;
	TeamCfgInfo*theCurrentTeamCfgData;//get from the teamcfg in robot,saves the team number and player number the robot is
	//RoboCupGameControlData *controlData;//final data;self data is in teams[0]
	/*LEDRequest*theLEDRequest;*/
	LEDRequest*theled; //-----old ledRequest
	RobotInfo* theRobotInfo;//self robot data
	bool checkPlaying;
	bool checkGameBegin;
	bool checkKickOff;
	bool checkPenalty;
	//perceiveInfo* theCurrentLocData;
#ifdef USE_BUTTON
	static const int NO_CLICKS;
	/*SmartPtr<ClickButton> chestButton,
		leftFootButton,
		rightFootButton;*/
#endif

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
