#include "RobotProvider.h"
#include "Platform/SystemCall.h"
#include "Platform/Win32/GetSetting.h"
#include "Platform/GetSetting.h"

#include <iostream>
#include <fstream>
#include <string>

#define  DEBUGING 0

#ifdef USE_BUTTON
const int BehaviorProvider::NO_CLICKS = -1;
#endif

BehaviorProvider::BehaviorProvider():checkPlaying(false),
checkGameBegin(true),
checkKickOff(true),
checkPenalty(false)
{
	std::cout<<"init Behavior"<<std::endl;
	theled = LEDRequest::getinstance();   //-----old ledRequest
	theCurrentRobocupGameData=new  RoboCupGameControlData();
	theCurrentMateInfo = new TeamMateInfo();
	initGameControl();
    theCurrentGlobalInfo=new GlobalInfo();
	theCurrentTeamCfgData=new TeamCfgInfo();
	theCurrentTeamCfgData->playerNum = 0;

    thePyRobot = new PyRobotInterface();

	readTeamConfig();
	std::cout<<"the Current TeamCfgData is "<<theCurrentTeamCfgData->playerNum<<std::endl;

}
void BehaviorProvider::readTeamConfig()
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
void BehaviorProvider::initGameControl()
{
	theCurrentRobocupGameData->teams[0].teamNumber = TJArkTeamNum;
	theCurrentRobocupGameData->teams[1].teamNumber = INACTIVE_TEAM_NUM;
	theCurrentRobocupGameData->kickOffTeam = INACTIVE_TEAM_NUM;
	theCurrentRobocupGameData->secondaryState = STATE2_NORMAL;
	theCurrentRobocupGameData->state = STATE_INITIAL;
	theCurrentRobocupGameData->teams[0].players[0].penalty = PENALTY_SPL_INACTIVE_PLAYER;
	theCurrentRobocupGameData->teams[0].players[1].penalty = PENALTY_SPL_INACTIVE_PLAYER;
	theCurrentRobocupGameData->teams[0].players[2].penalty = PENALTY_SPL_INACTIVE_PLAYER;
	theCurrentRobocupGameData->teams[1].players[0].penalty = PENALTY_SPL_INACTIVE_PLAYER;
	theCurrentRobocupGameData->teams[1].players[1].penalty = PENALTY_SPL_INACTIVE_PLAYER;
	theCurrentRobocupGameData->teams[1].players[2].penalty = PENALTY_SPL_INACTIVE_PLAYER;
}

BehaviorProvider::~BehaviorProvider()
{

	if(thePyRobot != NULL)
	{
		delete thePyRobot;
		thePyRobot = NULL;
	}
	if (theCurrentRobocupGameData!=NULL)
	{
		delete theCurrentRobocupGameData;
		theCurrentRobocupGameData=NULL;
	}

	if (theCurrentMateInfo!=NULL)
	{
		delete theCurrentMateInfo;
		theCurrentMateInfo=NULL;
	}

	if(theCurrentTeamCfgData != NULL)
	{
		delete theCurrentTeamCfgData;
		theCurrentTeamCfgData = NULL;
	}

}

void BehaviorProvider::inOutBehaviorData()
{


}

void BehaviorProvider::inTeamMateInfo()
{
	selfMessageQueue->SearchMyMessage(idTeamMateInfo,idNetworkThread,idRobotThread,
		(char*)theCurrentMateInfo,sizeof(TeamMateInfo));
	//std::cout<<"after search message"<<std::endl;
	//memcpy(theTeamMateInfo,theCurrentMateInfo,sizeof(TeamMateInfo));
}
void BehaviorProvider::inGlobalInfo()
{
	selfMessageQueue->SearchMyMessage(idGlobalInfo,idNetworkThread,idRobotThread,
		(char*)theCurrentGlobalInfo,sizeof(GlobalInfo));
}

void BehaviorProvider::outBehaviorData()
{
	indicators();   //-----old ledRequest
	runBehavior();
    	
	selfMessageQueue->SetMessageData(idMotionCmd,idMotionEngineWindow,idActuatorThread,
		sizeof(MotionCmd),(char*)theMotionCmd);

	selfMessageQueue->SetMessageData(idLocData,idRobotThread,idNetworkThread,
		sizeof(TeamUDPData),(char*)theTeamUDPData);
}

void BehaviorProvider::runBehavior()
{
	std::cout<<"run Behavior"<<std::endl;
	if(thePyRobot->initialModule == true)
	{
		thePyRobot->initializeVision(theGoalPercept,theBallPercept,theRobotPercept);
		thePyRobot->initializeLocator(theRobotPose,theBallModel,theFreePartOfOpponentGoalModel,theOdometryData);//,theBallModel);
		thePyRobot->initializeCommon(theOutBehaviorData);
		thePyRobot->initializeLED(theled);
		thePyRobot->initializeTeamCfg(theCurrentTeamCfgData);
		thePyRobot->initializeGameControl(theCurrentRobocupGameData,theCurrentTeamCfgData);
		thePyRobot->initializeMotion(theMotionCmd);
		thePyRobot->initializeTeamUDPDate(theTeamUDPData,theCurrentMateInfo);
        thePyRobot->initializeGlobalInfo(theCurrentGlobalInfo);
		thePyRobot->importmodules();
		thePyRobot->initialModule = false;//如果调试发现有python初始化的错误，就注掉这句，然后相当于每次都init一次py的实例，会每次报初始化的错误，调试结束应关掉
	}
	thePyRobot->updataVision(theGoalPercept,theBallPercept,theRobotPercept);
	thePyRobot->updateLocator(theRobotPose,theBallModel,theFreePartOfOpponentGoalModel,theOdometryData);//,theBallModel);
	thePyRobot->updateCommon(theOutBehaviorData);
	thePyRobot->updateGameControl(theCurrentRobocupGameData,theCurrentTeamCfgData);
	thePyRobot->processRun();
	thePyRobot->updateTeamCfg(theCurrentTeamCfgData);
	thePyRobot->updateLED(theled);
	thePyRobot->updateMoiton(theMotionCmd);
	thePyRobot->updateTeamUDPData(theTeamUDPData,theCurrentMateInfo);
    thePyRobot->updataGlobalInfo(theCurrentGlobalInfo);

	//**check penalty**//
	int tjarkNum=(uint32)theCurrentRobocupGameData->teams[0].teamNumber==TJArkTeamNum?0:1;
	uint16 penalty = theCurrentRobocupGameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty;
	if (penalty != 0)
	{
		checkPenalty = true;
	}
	if (checkPenalty)
	{
		if (penalty==0)
		{
			thePyRobot->reloadmodules();
			checkPenalty = false;
		}
	}
}

#ifdef USE_BUTTON
void BehaviorProvider::processChestButtonClicks()
{
	if(executeChestClicksAction(theOutBehaviorData->intData[3]))
	{
		//theOutBehaviorData->intData[3]= 0;
	}
}

bool BehaviorProvider::executeChestClicksAction(int nClicks)
{
	//std::cout<<"------processsing "<<nClicks<<"clicks"<<std::endl;
	switch(nClicks)
	{
	case NO_CLICKS:
		return false;
		break;
	case 6:
		//std::cout<<"@@@@@@@@@@@@@@@@"<<std::endl;
		thePyRobot->reloadmodules();
		break;
	//case 2:
	//	theled->led_off(LEDRequest::AllLeds);
	//	theled->led_on(LEDRequest::AllLedsGreen);
	//	break;
	//case 3:
	//	theled->led_off(LEDRequest::AllLeds);
	//	theled->led_on(LEDRequest::AllLedsRed);
	//	break;
	default:
		return false;
		break;
	}
	return true;
}
#endif

void BehaviorProvider::update(BehaviorData* theBehaviorData)
{
	std::cout<<"update Behav"<<std::endl;
	inRoboCupGameControlData();
	inTeamMateInfo();
	inGlobalInfo();
	//the function for buttonClicks
#ifdef USE_BUTTON
 	processChestButtonClicks();
#endif
	outBehaviorData();
	//std::cout<<"@@@@  after  upate(BehaviorData* theBehaviorData)  @@@@"<<std::endl;
}

void BehaviorProvider::update(GlobalInfo *theGlobalInfo)
{
	memcpy(theGlobalInfo,theCurrentGlobalInfo,sizeof(GlobalInfo));
}
void BehaviorProvider::update(Ballsamplebuffer *theBallsamplebuffer)
{

	Ballsamplebuffer::singleBallSample ss;
	ss.posX=int32(theCurrentGlobalInfo->lastBallLoc.x);
	ss.posY=int32(theCurrentGlobalInfo->lastBallLoc.y);
	ss.velocityX=int32(0);
	ss.velocityY=int32(0);
	ss.weighting=int32(0);
	int ballstate=0;
	ss.state=ballstate==0?Ballsamplebuffer::singleBallSample::LYING:Ballsamplebuffer::singleBallSample::ROLLING;
	ss.weighting=1;

	theBallsamplebuffer->BallParticleSamples[0] = ss;


}
//void BehaviorProvider::update(LEDRequest* theLEDRequest)
//{
////	if(theBallPercept->invion)
////	{
////		theLEDRequest->ledStates[LEDRequest::earsLeft0Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft36Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft72Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft108Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft144Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft180Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft216Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft252Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft288Deg] = LEDRequest::on;
////		theLEDRequest->ledStates[LEDRequest::earsLeft324Deg] = LEDRequest::on;
////	}
////	else
////	{
////		theLEDRequest->ledStates[LEDRequest::earsLeft0Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft36Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft72Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft108Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft144Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft180Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft216Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft252Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft288Deg] = LEDRequest::off;
////		theLEDRequest->ledStates[LEDRequest::earsLeft324Deg] = LEDRequest::off;
////	}
//}
/* checks the header of a packet. Since the GameController broadcasts 4
characters and not a string (which is terminated by a \0), we should check
each character individually instead of using something like strcmp */
bool BehaviorProvider::checkHeader(char* header)
{
	for (int i=0; i<4; i++){
		//cout<<"@@@"<<header[i]<<"@@@";
		//cout<<"###"<<GAMECONTROLLER_STRUCT_HEADER[i]<<"###";
		if (header[i] != GAMECONTROLLER_STRUCT_HEADER[i]) return false;}
	return true;
}

/* compare two byte streams, returns true (match) or false (no match) */
bool BehaviorProvider::dataEqual(RoboCupGameControlData* data, RoboCupGameControlData* previous) {
	if (!memcmp(previous, data, sizeof(RoboCupGameControlData)))
		return true;
	return false;
}
void BehaviorProvider::rawSwapTeams(RoboCupGameControlData* data) {
	size_t    teamSize = sizeof(TeamInfo);
	TeamInfo* blueTeam = &(data->teams[TEAM_BLUE]);
	TeamInfo* redTeam  = &(data->teams[TEAM_RED]);

	TeamInfo tempTeam;
	memcpy(&tempTeam, blueTeam, teamSize);

	/* swap the teams */
	memcpy(blueTeam, redTeam, teamSize);
	memcpy(redTeam, &tempTeam, teamSize);
	if(DEBUGING)
	{
		cout<<"======= in rawSwapTeams ====="<<endl;
		cout<<"team blue is "<<(uint32)data->teams[TEAM_BLUE].teamNumber<<endl;
	}

}
//包的核查
//bool BehaviorProvider::validatePacket(RoboCupGameControlData *data)
//{
//	cout<<"=============cout begin==============="<<std::endl;
//	/* check the right structure header has come in */
//	if (!(checkHeader(data->header)))
//	{
//		cout<<"DATA HEADER MISMATCH\n";
//		cout<<"local header is###"<<GAMECONTROLLER_STRUCT_HEADER<<" received header is###"<< data->header<<endl;
//		return false;
//	}
//	/* check for partial packets */
//	if (sizeof(*data) != sizeof(RoboCupGameControlData))
//	{
//		cout<<"RECEIVED PARTIAL PACKET\n";
//		cout<< "data: " << sizeof(*data) << " " << "controller: " << sizeof(RoboCupGameControlData) << endl;
//		return false;
//	}
//	/* check the right version of the structure is being used */
//	if (data->version != GAMECONTROLLER_STRUCT_VERSION)
//	{
//		cout<<"DATA VERSION MISMATCH\n";
//		cout<<"local version is version %d, receiving version %d",
//			GAMECONTROLLER_STRUCT_VERSION, data->version;
//		return false;
//	}
//	/* check whether this packet belongs to this game at all */
//	if (!isThisGame(data)) return false;
//
//	return true;
//
//}
////取得包并核查是否我们的包，然后处理数据

void BehaviorProvider::inRoboCupGameControlData()
{
	selfMessageQueue->SearchMyMessage(idGameControlData,idNetworkThread,idRobotThread,
		(char*)theCurrentRobocupGameData,sizeof(RoboCupGameControlData));

	//if we can't connect the network, give GameControlData by manually

	manualGameControl(theCurrentRobocupGameData);
	GCIndicators(theCurrentRobocupGameData);
	//processGameData(theCurrentRobocupGameData);

	//because we should send it to the BlackBoard, so it should not be swapped!!!
	/*if (int(theCurrentRobocupGameData->teams[TEAM_BLUE].teamNumber )!=TJArkTeamNum )
		rawSwapTeams(theCurrentRobocupGameData);*/
	/*if (!dataEqual(theCurrentRobocupGameData, controlData))
	{
		memcpy(controlData, theCurrentRobocupGameData, sizeof(RoboCupGameControlData));
	}*/

}

void BehaviorProvider::manualGameControl(RoboCupGameControlData* gameData)
{

	
	if(theOutBehaviorData->intData[2] == 1 && checkGameBegin)
	{
		gameData->teams[0].teamNumber = uint8(INACTIVE_TEAM_NUM);
		gameData->teams[1].teamNumber = uint8(TJArkTeamNum);
		checkGameBegin = false;
	}
	if(theOutBehaviorData->intData[1] == 1 && checkKickOff)
	{
		int tjarkNum=(uint32)gameData->teams[0].teamNumber==TJArkTeamNum?0:1;
		gameData->kickOffTeam = uint8(tjarkNum);
		checkKickOff = false;
		checkGameBegin = false;
	}
	if(theOutBehaviorData->intData[3] == 1 && !checkPlaying)
	{

		gameData->state = uint8(STATE_PLAYING);
		gameData->teams[0].players[theCurrentTeamCfgData->playerNum-1].penalty = uint16(PENALTY_NONE);
		gameData->teams[1].players[theCurrentTeamCfgData->playerNum-1].penalty = uint16(PENALTY_NONE);
		checkPlaying = true;
		checkGameBegin = false;
		checkKickOff = false;
	}
	else if(theOutBehaviorData->intData[3] == 1 && checkPlaying)
	{
		gameData->teams[0].players[theCurrentTeamCfgData->playerNum-1].penalty = uint16(PENALTY_SPL_INACTIVE_PLAYER);
		gameData->teams[0].players[theCurrentTeamCfgData->playerNum-1].secsTillUnpenalised = 30;
		gameData->teams[1].players[theCurrentTeamCfgData->playerNum-1].penalty = uint16(PENALTY_SPL_INACTIVE_PLAYER);
		gameData->teams[1].players[theCurrentTeamCfgData->playerNum-1].secsTillUnpenalised = 30;
		checkPlaying = false;
	}
}

void BehaviorProvider::GCIndicators(RoboCupGameControlData* gameData)
{
	int tjarkNum=(uint32)gameData->teams[0].teamNumber==TJArkTeamNum?0:1;
	if(tjarkNum == 0)
		theled->on(LEDRequest::LeftFootLedsBlue);
	else
		theled->on(LEDRequest::LeftFootLedsRed);

	if(int(gameData->kickOffTeam) == tjarkNum)
		theled->led_on(LEDRequest::RightFootLeds);
	else
		theled->led_off(LEDRequest::RightFootLeds);

	if(int(gameData->state) == STATE_INITIAL)
		theled->led_off(LEDRequest::ChestLeds);
	else if(int(gameData->state) == STATE_READY)
	{
		theled->led_off(LEDRequest::ChestLeds);
		theled->on(LEDRequest::ChestLedsBlue);
	}
	else if(int(gameData->state) == STATE_SET)
	{
		theled->led_off(LEDRequest::ChestLeds);
		theled->on(LEDRequest::ChestLedsGreen);
		theled->on(LEDRequest::ChestLedsRed);
	}
	else if(int(gameData->state) == STATE_PLAYING)
	{
		theled->led_off(LEDRequest::ChestLeds);
		theled->on(LEDRequest::ChestLedsGreen);
	}
	else if(int(gameData->state) == STATE_FINISHED)
	{
		theled->led_off(LEDRequest::ChestLeds);
	}

	std::cout<<gameData->teams[0].players[0].penalty<<" "<<gameData->teams[0].players[1].penalty<<" "<<gameData->teams[0].players[2].penalty<<" "<<gameData->teams[0].players[3].penalty<<std::endl;
	if((int(gameData->state) == STATE_PLAYING) &&
		(int(gameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty) != PENALTY_NONE))
	{
		theled->led_off(LEDRequest::ChestLeds);
		theled->on(LEDRequest::ChestLedsRed);
	}
}

void BehaviorProvider::processGameData(RoboCupGameControlData* gameData)
{
	if (DEBUGING)
	{
		cout<<"!!!!!Begin processGameData!!!!"<<endl;
	}
	/* check which half the game is in */
	if (int(gameData->firstHalf) == 1)
	{
		if (DEBUGING) cout << "1st half \n";
	} else {
		if (DEBUGING) cout << "2nd half \n";
	}
	/* check what state the game is in */
	if (int(gameData->state) == STATE_INITIAL) {
		if (DEBUGING) cout << "@@@@@@(INITIAL)@@@@@@@\n";
	} else if (int(gameData->state) == STATE_READY) {
		if (DEBUGING) cout << "@@@@@@@(READY)@@@@@@@ \n";
	} else if (int(gameData->state) == STATE_SET) {
		if (DEBUGING) cout << "@@@@@@@(SET)@@@@@@@ \n";
	} else if (int(gameData->state) == STATE_PLAYING) {
		if (DEBUGING) cout << "@@@@@@@(PLAYING)@@@@@@@ \n";
	} else if (int(gameData->state) == STATE_FINISHED) {
		if (DEBUGING) cout << "@@@@@@@(FINISHED)@@@@@@@ \n";
	}
// 	/* look at who is kicking off */
// 	if (int(gameData->kickOffTeam) == theCurrentTeamCfgData->teamNum)//若参赛队“不”只有0、1编号，此处需调整
// 	{
// 		if (DEBUGING) cout << "OWN kickoff\n";
// 	}
// 	else
// 	{
// 		if (DEBUGING) cout << "OTHER kickoff\n";
// 	}
	/* Is it a penalty shootout? */
	//cout<<"###secondaryState==="<<gameData->secondaryState<<endl;
	//cout<<"###penalty=========="<<gameData->teams[0].players[theCurrentTeamCfgData->playerNum-1].penalty<<endl;
	//player number in the robot starts from 1,while the player 数组 starts from 0, so when be used, player number should -1;
	if (int(gameData->secondaryState) != STATE2_NORMAL) {
		if (DEBUGING) cout << "Secondary state: "
			<< gameData->secondaryState << endl;
	}
	/* look at my penalty */
	int tjarkNum=(uint32)gameData->teams[0].teamNumber==TJArkTeamNum?0:1;
    if(DEBUGING)
	   cout<<"tjarkNum is "<<tjarkNum<<endl;
	if(DEBUGING)
	{
		cout<<"theCurrentTeamCfgData->playerNum = "<<theCurrentTeamCfgData->playerNum<<endl;
		cout<<"penaly = "<<int(gameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty)<<endl;
	}
	if ( int(gameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty)== PENALTY_NONE)
	{


		if (DEBUGING) cout << "I am NOT PENALISED, ";
	}
	else
	{
		switch (int(gameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty))
		{
		case PENALTY_SPL_BALL_HOLDING:
			if (DEBUGING) cout << "PENALTY_SPL_BALL_HOLDING for ";
			break;
		case PENALTY_SPL_PLAYER_PUSHING:
			if (DEBUGING) cout << "PENALTY_SPL_PLAYER_PUSHING for ";
			break;
		case PENALTY_SPL_OBSTRUCTION:
			if (DEBUGING) cout << "PENALTY_SPL_OBSTRUCTION for ";
			break;
		case PENALTY_SPL_INACTIVE_PLAYER:
			if (DEBUGING) cout << "PENALTY_SPL_INACTIVE_PLAYER for ";
			break;
		case PENALTY_SPL_ILLEGAL_DEFENDER:
			if (DEBUGING) cout << "PENALTY_SPL_ILLEGAL_DEFENDER for ";
			break;
		case PENALTY_SPL_LEAVING_THE_FIELD:
			if (DEBUGING) cout << "PENALTY_SPL_LEAVING_THE_FIELD for ";
			break;
		case PENALTY_SPL_PLAYING_WITH_HANDS:
			if (DEBUGING) cout << "PENALTY_SPL_PLAYING_WITH_HANDS for ";
			break;
		case PENALTY_SPL_REQUEST_FOR_PICKUP:
			if (DEBUGING) cout << "PENALTY_SPL_REQUEST_FOR_PICKUP for ";
			break;
		case PENALTY_MANUAL:
			if (DEBUGING) cout << "PENALTY_MANUAL for ";
			break;
		default:
			if (DEBUGING) cout << "Unknown penalty for ";
			break;
		}
		if (DEBUGING) cout << gameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].secsTillUnpenalised << " secs&&&&&&&&&&&&\n ";
	}
	/* look at remaining time */
	if (DEBUGING) cout << gameData->secsRemaining << " secs remaining$$$$$$$$$$$" << endl;
}
void BehaviorProvider::update(RoboCupGameControlData* theRoboCupGameControlData)
{
	memcpy(theRoboCupGameControlData,theCurrentRobocupGameData,sizeof(RoboCupGameControlData));
}
void BehaviorProvider::update(TeamCfgInfo* theTeamCfgInfo)
{
	theTeamCfgInfo->playerNum = theCurrentTeamCfgData->playerNum;
}

void BehaviorProvider::indicators()
{
	
	if(theBallPercept->invion)
		theled->led_on(LEDRequest::EarLeds);
	else
		theled->led_off(LEDRequest::EarLeds);
	if (theGoalPercept->getoppleftgoalseen()&&theGoalPercept->getopprightgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::LeftFaceLedsRed);
		theled->led_on(LEDRequest::RightFaceLedsRed);
	}
	else if(theGoalPercept->getoppleftgoalseen()&&!theGoalPercept->getopprightgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::LeftFaceLedsRed);
	}
	else if(theGoalPercept->getopprightgoalseen()&&!theGoalPercept->getoppleftgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::RightFaceLedsRed);
	}
	else
	{
		theled->led_off(LEDRequest::FaceLeds);
	}
	if (theGoalPercept->getownleftgoalseen()&&theGoalPercept->getownrightgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::LeftFaceLedsBlue);
		theled->led_on(LEDRequest::RightFaceLedsBlue);
	}
	else if(theGoalPercept->getownleftgoalseen()&&!theGoalPercept->getownrightgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::LeftFaceLedsBlue);
	}
	else if(theGoalPercept->getownrightgoalseen()&&!theGoalPercept->getownleftgoalseen())
	{
		theled->led_off(LEDRequest::FaceLeds);
		theled->led_on(LEDRequest::RightFaceLedsBlue);
	}
}
void BehaviorProvider::update(RobotInfo* theRobotInfo)
{
	int tjarkNum=(uint32)theCurrentRobocupGameData->teams[0].teamNumber==TJArkTeamNum?0:1;
	theRobotInfo->penalty = theCurrentRobocupGameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].penalty;
	theRobotInfo->secsTillUnpenalised = theCurrentRobocupGameData->teams[tjarkNum].players[theCurrentTeamCfgData->playerNum-1].secsTillUnpenalised;
}
void BehaviorProvider::update(TeamUDPData* theTeamUDPData)
{
	bool sended;
	if(selfMessageQueue->SearchMyMessage(idSended,idRobotThread,idNetworkThread,(char*)(&sended),sizeof(bool)))
	{
		theTeamUDPData->send = false;
	}
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

MAKE_KS(BehaviorProvider)


