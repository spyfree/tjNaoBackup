#include "KeeperMotion.h"
#include "Platform/Win32/GetSetting.h"
#include "Platform/GetSetting.h"

#include <iostream>
#include <fstream>
#include <string>

keeperMotion::keeperMotion():teamNum(0)
{
	theCurrentBallModel = new BallModel();
	getTeamNum();
}
keeperMotion::~keeperMotion()
{

}

void keeperMotion::getTeamNum()
{
	std::string buffer;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/team.cfg";
	std::ifstream stream(path.c_str());
	std::cout<<path<<std::endl;
	if (!stream.is_open())
	{
		std::cout<<path<<" Open team configure file failed "<<std::endl;
	}
	stream>>teamNum;
}

void keeperMotion::inBallModel()
{
	selfMessageQueue->SearchMyMessage(idBallModel,idRobotThread,idActuatorThread,
		(char*)theCurrentBallModel,sizeof(BallModel));
}

int keeperMotion::shouldDive()
{
	inBallModel();
	std::cout<<"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"<<theCurrentBallModel->estimate.velocity.x<<std::endl;
	if (theCurrentBallModel->estimate.velocity.x > 0)
	{
		if(theCurrentBallModel->estimate.velocity.y > 0.08)
			diveLeft();
		else if(theCurrentBallModel->estimate.velocity.y < -0.08)
			diveRight();
	}
}

void keeperMotion::run()
{
	if (teamNum!=1)
	{
		;
	}
	else
	{
		shouldDive();
	}
}

void keeperMotion::diveLeft()
{
	myProxyContainer.ttsProxy->say("Left");
}

void keeperMotion::diveRight()
{
	myProxyContainer.ttsProxy->say("Right");
}