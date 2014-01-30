#ifndef RunSimBehav_H
#define RunSimBehav_H
#include "boost/shared_ptr.hpp"
#include "Module/Vision/PyVision.h"
#include "Module/Common/PyCommon.h"
#include "Module/Actuator/PyMotion.h"
#include "Module/Network/PyGameControl.h"
#include "Module/Network/PyTeamCfg.h"
#include "Module/Common/PySimLED.h"
#include "Module/Modeling/PyLocator.h"
//#include "Module/Network/PyTeammate.h"
#include <Python.h>

class PyRobotSimInterface
{
public:
	PyRobotSimInterface();
	~PyRobotSimInterface();
	void initializeVision(const GoalPercept*theGoalData,const BallPercept*theBallData,const RobotPercept*theRobotData);
	void initializeCommon(const OutBehaviorData *_theOutBehaviorData);
	void initializeMotion(MotionCmd *_theMotionCmd);
	void initializeTeamCfg(TeamCfgInfo *_theTeamData);
	void initializeGameControl(const RoboCupGameControlData *_theGameControlData,
		const TeamCfgInfo*_theTeamCfgInfoData);
	 void initializeLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel);
	//void initializeTeammate(const perceiveInfo* theLocData,const TeamMateInfo* theTeaMateData);
	void initializeLED(LEDSimRequest *_theLEDData);
	void modifySysPath();
	void importmodules();
	void reloadmodules();//reload python files
	void getBehavInstance();
	void processRun();
	void updataVision(const GoalPercept*theGoalData,const BallPercept*theBallData,const RobotPercept*theRobotData);
	//void updateLocator(const RobotPose* theRobotPoseData);//,const BallModel* theBallModelData);
	void updateLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel);
	void updateCommon(const OutBehaviorData *_theOutBehaviorData);
	void updateMoiton(MotionCmd *_theMotionData);
	void updateTeamCfg(TeamCfgInfo *_theTeamData);
	void updateLED(LEDSimRequest *_theLEDData);
	void updateGameControl(const RoboCupGameControlData*_theGameControlData,
		const TeamCfgInfo *_theTeamCfgInfoData);
	//void updateTeammate(const perceiveInfo* _theLocData,const TeamMateInfo* theTeaMateData);
public:
	bool initialModule;
private:
	PyObject *behav_module;
	PyObject *behav_instance;
};
#endif
