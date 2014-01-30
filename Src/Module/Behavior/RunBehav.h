#ifndef RUNBEHAV_H
#define RUNBEHAV_H
#include "boost/shared_ptr.hpp"
#include "Module/Vision/PyVision.h"
#include "Module/Network/PyGlobalInfo.h"
#include "Module/Common/PyCommon.h"
#include "Module/Representations/Perception/PyCamera.h"
#include "Module/Actuator/PyMotion.h"
#include "Module/Network/PyGameControl.h"
#include "Module/Network/PyTeamCfg.h"
#include "Module/Network/PyTeamUDP.h"
#include "Module/Common/PyLED.h"
#include "Module/Modeling/PyLocator.h"
#include "Module/Network/networkComm/WdataInfo.h"
//#include "Module/Network/PyTeammate.h"
#include <Python.h>

class PyRobotInterface
{
public:
	PyRobotInterface();
	~PyRobotInterface();
	void initializeVision(const GoalPercept*theGoalData,const BallPercept*theBallData,const RobotPercept*theRobotData);
	void initializeGlobalInfo(GlobalInfo* theGlobalInfo);
	void initializeCommon(const OutBehaviorData *_theOutBehaviorData);
	void initializeMotion(MotionCmd *_theMotionCmd);
	void initializeTeamUDPDate(TeamUDPData *_theTeamUDPData,TeamMateInfo *_teammateData);
	void initializeCamera(const CameraConfig *theCameraData);
	void initializeTeamCfg(TeamCfgInfo *_theTeamData);
	void initializeGameControl(const RoboCupGameControlData *_theGameControlData,
		const TeamCfgInfo*_theTeamCfgInfoData);
    void initializeLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel,const OdometryData*theOdometryData);
	void initializeLED(LEDRequest *_theLEDData);
	void modifySysPath();
	void importmodules();
	void reloadmodules();//reload python files
	void getBehavInstance();
	void processRun();
	void updataVision(const GoalPercept*theGoalData,const BallPercept*theBallData,const RobotPercept*theRobotData);
    void updataGlobalInfo(GlobalInfo* theGlobalInfo);
	void updateLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel,const OdometryData*theOdometryData);
	void updateCommon(const OutBehaviorData *_theOutBehaviorData);
	void updateMoiton(MotionCmd *_theMotionData);
	void updateTeamUDPData(TeamUDPData *_theTeamUDPData,TeamMateInfo *_teammateData);
	void updateCamera(const CameraConfig *_theCameraData);
	void updateTeamCfg(TeamCfgInfo *_theTeamData);
	void updateGameControl(const RoboCupGameControlData*_theGameControlData,
		const TeamCfgInfo *_theTeamCfgInfoData);
	void updateLED(LEDRequest *_theLEDData);
private:
	PyObject *behav_module;
	PyObject *behav_instance;
public:
	bool initialModule;
};
#endif
