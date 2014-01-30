#include "NetworkProvider.h"
#include <iostream>
//PROCESS_WIDE_STORAGE NetworkProvider* NetworkProvider::theInstance = 0;
NetworkProvider::NetworkProvider()
{	//NetworkProvider::theInstance = this;
	udpPoint.bind();
	gameControl.bind();
	ownTeamInfo  = new TeamUDPData();
}
NetworkProvider::~NetworkProvider()
{
 //NetworkProvider::theInstance = 0;
	udpPoint.close();
	gameControl.close();
}


void NetworkProvider::update(RoboCupGameControlData*theRoboCupGameControlData)
{
	//std::cout<<"@@@@@@@@@@@@@@@@@@2robotcupgamecontrol###########"<<std::endl;
	inTeamCfgInfo();
	RoboCupGameControlData gameControlData;
	gameControlData.teams[0].teamNumber=-1;
	gameControlData.teams[1].teamNumber=-1;
	bool ret=gameControl.read(gameControlData);
	//std::cout<<"ret is "<<ret<<std::endl;

    if(isThisGame(gameControlData))
	{
		//std::cout<<"isThisGame gameControlData team0 number is "<<std::endl;
		//if (int(gameControlData.teams[TEAM_BLUE].teamNumber)!=teamNum)
		//{
		//	//rawSwapTeams(gameControlData);
		//	std::cout<<"run swapteams -----"<<std::endl;
		//}
	    memcpy(theRoboCupGameControlData,&(gameControlData),sizeof(RoboCupGameControlData));
		outRoboCupGameControlData();
	}

	//std::cout<<"=====in updateRoboCupGameControlData===="<<std::endl;
	//std::cout<<"the RoboCupGameControlData playersPerTeam is "<<(uint32)theRoboCupGameControlData->playersPerTeam<<std::endl;
	//std::cout<<"the RoboCupGameControlData state is "<<(uint32)theRoboCupGameControlData->state<<std::endl;
	//std::cout<<"the RoboCupGameControlData firstHalf is "<<(uint32)theRoboCupGameControlData->firstHalf<<std::endl;
	//std::cout<<"the RoboCupGameControlData kickOffTeam is "<<(uint32)theRoboCupGameControlData->kickOffTeam<<std::endl;
	//std::cout<<"the RoboCupGameControlData secondaryState is "<<(uint32)theRoboCupGameControlData->secondaryState<<std::endl;
	//std::cout<<"the RoboCupGameControlData dropInTeam is "<<(uint32)theRoboCupGameControlData->dropInTeam<<std::endl;
	//std::cout<<"the RoboCupGameControlData secsRemaining is "<<theRoboCupGameControlData->secsRemaining<<std::endl;
}
bool NetworkProvider::isThisGame(RoboCupGameControlData& gameData)
{

	if (int(gameData.teams[TEAM_BLUE].teamNumber) != this->teamNum &&
		int(gameData.teams[TEAM_RED].teamNumber)  != this->teamNum)
	{
			std::cout<< "Got packet not for this game\n" <<std::endl;
			return false;
	}
	return true;

}
//void NetworkProvider::rawSwapTeams(RoboCupGameControlData& data)
//{
//	size_t    teamSize = sizeof(TeamInfo);
//	TeamInfo* blueTeam = &(data.teams[TEAM_BLUE]);
//	TeamInfo* redTeam  = &(data.teams[TEAM_RED]);
//
//	TeamInfo tempTeam;
//	memcpy(&tempTeam, blueTeam, teamSize);
//
//	/* swap the teams */
//	memcpy(blueTeam, redTeam, teamSize);
//	memcpy(redTeam, &tempTeam, teamSize);
//}
//bool NetworkProvider::dataEqual(RoboCupGameControlData* data, RoboCupGameControlData* previous) {
//	if (!memcmp(previous, data, sizeof(RoboCupGameControlData)))
//		return true;
//	return false;
//}
void NetworkProvider:: outRoboCupGameControlData()
{
	//std::cout<<"in outRoboCupGameControlData "<<std::endl;
	selfMessageQueue->SetMessageData(idGameControlData,idNetworkThread,idRobotThread,
		sizeof(RoboCupGameControlData),(char*)theRoboCupGameControlData);
}
//void NetworkProvider::updateNetworkData(RoboCupGameControlData theControlData,TeamMateInfo theNetTeamData)
//{
//	theInstance->theNetworkGameData=theControlData;
//	theInstance->theNetTeamMateData=theNetTeamData;
//
//
//}
void NetworkProvider::update(TeamMateInfo *theTeamMateInfo)
{

	// if own teaminfo needs to be sent ,send it!
	//std::cout<<"update teammate info ############# "<<std::endl;

	unsigned mateInfoprocesstime=SystemCall::getCurrentSystemTime();

	if(selfMessageQueue->SearchMyMessage(idLocData,idRobotThread,idNetworkThread,
		(char*)ownTeamInfo,sizeof(TeamUDPData)))
	{
        TeamUDPData ownTeam;
		memcpy(&ownTeam,ownTeamInfo,sizeof(TeamUDPData));
		udpPoint.setOwnTeamInfo(&ownTeam);
		if(ownTeam.send)
		{
			playerNum = ownTeam.role;
			udpPoint.send();
			bool sended = true;		
			selfMessageQueue->SetMessageData(idSended,idRobotThread,idNetworkThread,sizeof(bool),(char*)(&sended));
		}
	}
	else
	{
		std::cout<<"the team mate info not found"<<std::endl;
	}


	bool getMateInfo(false);
	int ret=udpPoint.select();
	if(ret==SOCKET_ERROR)
	{
		std::cout<<"receive select error"<<std::endl;
	}
	if(ret==0)
	{
		std::cout<<"receive select time out"<<std::endl;
	}
	else if(ret>0)
	{
		//std::cout<<"select sucess ,read data"<<std::endl;
		if(udpPoint.insert())
			getMateInfo=udpPoint.readdata(teamMateInfoData);
	}

	//if(getMateInfo)
	//{
	//	memcpy(theTeamMateInfo,&teamMateInfoData,sizeof(TeamMateInfo));
	//	outTeamMateInfo();
	//}
	//else
	//{
	//	memset(theTeamMateInfo,0,sizeof(TeamMateInfo));
	//	outTeamMateInfo();
	//}

	memcpy(theTeamMateInfo,&teamMateInfoData,sizeof(TeamMateInfo));
	outTeamMateInfo();

	unsigned mateProcesstimeused=SystemCall::getTimeSince(mateInfoprocesstime);
	std::cout<<"mateInfo update process time--:"<<mateProcesstimeused<<" ms"<<std::endl;



}
void NetworkProvider::outTeamMateInfo()
{
	selfMessageQueue->SetMessageData(idTeamMateInfo,idNetworkThread,idRobotThread,
		sizeof(TeamMateInfo),(char*)theTeamMateInfo);

}


void NetworkProvider::inTeamCfgInfo()
{
	//this->teamNum=theTeamCfgInfo->teamNum;
	this->teamNum=TJArkTeamNum;
	//std::cout<<"===========in NetworkProvider, the reamNum is ======="<<(uint32)this->teamNum<<std::endl;
}

NetworkManager::NetworkManager()
{
	blackboard=new Blackboard;
}
NetworkManager::~NetworkManager()
{
	if(blackboard)
	{
		delete blackboard;
		blackboard=NULL;
	}

}
void NetworkManager::initNetworkKs()
{
	setGlobal();
	initKSManager();
	updateKSManager();
}
void NetworkManager::executeNetKs()
{
	theKSManager->execute();
}
void NetworkManager::destoryNetKs()
{
	theKSManager->destroy();
}
void NetworkManager::setGlobal()
{
	Blackboard::theInstance=blackboard;
}
void NetworkManager::initKSManager()
{
	theKSManager=SmartPtr<KSManager>(new KSManager("NetworkThread"));
}
void NetworkManager::updateKSManager()
{
	theKSManager->update(0);
}

MAKE_KS(NetworkProvider)
