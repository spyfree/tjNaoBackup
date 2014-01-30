#include "RobotSimProvider.h"
#include "Platform/SystemCall.h"
#include "Platform/Win32/GetSetting.h"
#include "Platform/GetSetting.h"

#include <iostream>
#include <fstream>
#include <string>

BehaviorSimProvider::BehaviorSimProvider()
{
	std::cout<<"init Behav"<<std::endl;
	//theCurrentLocData = new perceiveInfo();
	thePyRobot = new PyRobotSimInterface();
	theled = LEDSimRequest::getinstance();
	theCurrentTeamCfgData = new TeamCfgInfo();
	readTeamConfig();
	std::cout<<"end of sim provider"<<std::endl;
}
BehaviorSimProvider::~BehaviorSimProvider()
{

	if(thePyRobot != NULL)
	{
		delete thePyRobot;
		thePyRobot = NULL;
	}
	if(theCurrentTeamCfgData!=NULL)
	{
		delete theCurrentTeamCfgData;
		theCurrentTeamCfgData = NULL;
	}
	//if(theCurrentLocData!=NULL)
	//{
	//	delete theCurrentLocData;
	//	theCurrentLocData=NULL;
	//}
}
void BehaviorSimProvider::readTeamConfig()
{
	std::string buffer;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/team.cfg";
	std::ifstream stream(path.c_str());
	std::cout<<path<<std::endl;
	if (!stream.is_open())
	{
		std::cout<<path<<" Open team configure file failed "<<std::endl;
	}
	stream>>theCurrentTeamCfgData->playerNum;

}
void BehaviorSimProvider::inOutBehaviorData()
{
	std::cout<<"fallstate when getting"<<theOutBehaviorData->intData[0]<<std::endl;

}

//void BehaviorSimProvider::inTeamMateInfo()
//{
//	
//}

void BehaviorSimProvider::outBehaviorData()
{
	/*std::cout<<"BehaviorSimProvider------ballX = "<<theVisionSimInterface->visionSimData.getBallX()<<std::endl;
	std::cout<<"BehaviorSimProvider------ballY = "<<theVisionSimInterface->visionSimData.getBallY()<<std::endl;
	std::cout<<"BehaviorSimProvider------globalX = "<<theVisionSimInterface->visionSimData.getGlobalX()<<std::endl;
	std::cout<<"BehaviorSimProvider------globalY = "<<theVisionSimInterface->visionSimData.getGlobalY()<<std::endl;
	std::cout<<"BehaviorSimProvider------distToBall = "<<theVisionSimInterface->visionSimData.getDistToBall()<<std::endl;
	std::cout<<"BehaviorSimProvider------direcToBall = "<<theVisionSimInterface->visionSimData.getDirecToBall()<<std::endl;*/
	runBehavior();
	//std::cout<<"-+-+-+-+-+-+-+-+ motionID = "<<theMotionCmd->motion<<std::endl;
	//std::cout<<"-+-+-+-+-+-+-+-+ walkType = "<<theMotionCmd->walkCmd.walkType<<std::endl;
	//std::cout<<"B+-+-+-+-+-+-+ speedX = "<<theMotionCmd->walkCmd.speed.translation.x<<std::endl;
	//std::cout<<"B+-+-+-+-+-+-+ speedY = "<<theMotionCmd->walkCmd.speed.translation.y<<std::endl;
	//std::cout<<"B+-+-+-+-+-+-+ speedZ = "<<theMotionCmd->walkCmd.speed.rotation<<std::endl;
	//std::cout<<"---------------pan = "<<theMotionCmd->headCmd.pan<<std::endl;
	//std::cout<<"---------------tilt = "<<theMotionCmd->headCmd.tilt<<std::endl;
	//std::cout<<"---------------speed = "<<theMotionCmd->headCmd.speed<<std::endl;
	//std::cout<<"---------------camera = "<<theCameraConfig->usingTopCam<<std::endl;

	selfMessageQueue->SetMessageData(idMotionCmd,idMotionEngineWindow,idActuatorThread,
		sizeof(MotionCmd),(char*)theMotionCmd);
}

void BehaviorSimProvider::runBehavior()
{
	if(thePyRobot->initialModule == true)
	{
		thePyRobot->initializeVision(theGoalPercept,theBallPercept,theRobotPercept);
		//thePyRobot->initializeLocator(theRobotPose,theBallModel,theFreePartOfOpponentGoalModel);//,theBallModel);
		thePyRobot->initializeCommon(theOutBehaviorData);
		thePyRobot->initializeLED(theled);
		thePyRobot->initializeTeamCfg(theCurrentTeamCfgData);
		thePyRobot->initializeGameControl(theRoboCupGameControlData,theCurrentTeamCfgData);
		//thePyRobot->initializeTeammate(theCurrentLocData,theTeamMateInfo);
		thePyRobot->initializeMotion(theMotionCmd);
		thePyRobot->importmodules();
		thePyRobot->initialModule = false;
	}
	thePyRobot->updataVision(theGoalPercept,theBallPercept,theRobotPercept);
	thePyRobot->updateLocator(theRobotPose,theBallModel,theFreePartOfOpponentGoalModel);//,theBallModel);
	thePyRobot->updateCommon(theOutBehaviorData);
	thePyRobot->updateGameControl(theRoboCupGameControlData,theCurrentTeamCfgData);
	thePyRobot->processRun();
	thePyRobot->updateTeamCfg(theCurrentTeamCfgData);
	thePyRobot->updateLED(theled);
	//thePyRobot->updateTeammate(theCurrentLocData,theTeamMateInfo);
	thePyRobot->updateMoiton(theMotionCmd);
}

void BehaviorSimProvider::update(BehaviorData* theBehaviorData)
{
	//std::cout<<"update Behav"<<std::endl;
	inRoboCupGameControlData();
	//inTeamMateInfo();
	outBehaviorData();
}
void BehaviorSimProvider::update(TeamCfgInfo* theTeamCfgInfo)
{
	theTeamCfgInfo->playerNum = theCurrentTeamCfgData->playerNum;
}

void BehaviorSimProvider::inRoboCupGameControlData()
{
	theRoboCupGameControlData->state = STATE_PLAYING;
	theRoboCupGameControlData->teams[0].teamNumber= TJArkTeamNum;
	theRoboCupGameControlData->teams[0].players[0].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[0].players[1].penalty=PENALTY_NONE;
    theRoboCupGameControlData->teams[0].players[2].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[0].players[11].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[0].score=0;
	theRoboCupGameControlData->teams[1].players[0].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[1].players[1].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[1].players[2].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[1].players[11].penalty=PENALTY_NONE;
	theRoboCupGameControlData->teams[1].score=0;
	theRoboCupGameControlData->kickOffTeam =TEAM_BLUE;
	theRoboCupGameControlData->secondaryState = STATE2_NORMAL;

}

void BehaviorSimProvider::update(RobotInfo* theRobotInfo)
{
	theRobotInfo->penalty = theRoboCupGameControlData->teams[0].players[theCurrentTeamCfgData->playerNum-1].penalty;
	theRobotInfo->secsTillUnpenalised = theRoboCupGameControlData->teams[0].players[theCurrentTeamCfgData->playerNum-1].secsTillUnpenalised;
}

//void BehaviorSimProvider::update(TeamCfgInfo* theTeamCfgInfo)
//{
//
//}

void BehaviorSimProvider::update(RoboCupGameControlData* theRoboCupGameControlData)
{

}

RobotManager::RobotManager()
{
	blackboard=new Blackboard;
}
RobotManager::~RobotManager()
{
	if(blackboard)
	{
		delete blackboard;
		blackboard=NULL;
	}
}
void RobotManager::initRobotKs()
{
	setGlobal();
	initKSManager();
	updateKSManager();
}
void RobotManager::executeRobKs()
{
	theKSManager->execute();
}
void RobotManager::destoryRobKs()
{
	theKSManager->destroy();
}
void RobotManager::setGlobal()
{
	Blackboard::theInstance=blackboard;
}
void RobotManager::initKSManager()
{
	theKSManager=new KSManager("TJRobotThread");
}
void RobotManager::updateKSManager()
{
	theKSManager->update(0);
}


MAKE_KS(BehaviorSimProvider)
