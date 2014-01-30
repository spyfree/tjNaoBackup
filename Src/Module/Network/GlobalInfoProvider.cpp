#include "GlobalInfoProvider.h"
#include <math.h>

GlobalInfoProvider::GlobalInfoProvider():ball1Counter(0),ball2Counter(0),ball3Counter(0),ball4Counter(0),kickCounter(0),keeperSeenBallCounter(0)
{

}
GlobalInfoProvider::~GlobalInfoProvider()
{

}
void GlobalInfoProvider::inTeamMateInfo()
{

}

void GlobalInfoProvider::update(GlobalInfo *theGlobalInfo)
{
	theGlobalInfo->keeperLoc = theTeamMateInfo->teamMateInfo1.robotLocation;
	theGlobalInfo->attack1Loc = theTeamMateInfo->teamMateInfo2.robotLocation;
	theGlobalInfo->attack2Loc = theTeamMateInfo->teamMateInfo4.robotLocation;
	theGlobalInfo->defenderLoc = theTeamMateInfo->teamMateInfo3.robotLocation;

	int kickNum = 0;
	if (theTeamMateInfo->teamMateInfo1.kickPlayer != 0)
	{
		kickNum = 1;
	}
	else if (theTeamMateInfo->teamMateInfo2.kickPlayer != 0)
	{
		kickNum = 2;
	}
	else if(theTeamMateInfo->teamMateInfo3.kickPlayer != 0)
	{
		kickNum = 3;
	}
	else if(theTeamMateInfo->teamMateInfo4.kickPlayer != 0)
	{
		kickNum = 4;
	}
	else
	{
		kickNum = 0;
	}
	
	if (kickNum != 0)
	{
		theGlobalInfo->kickerNum = kickNum;
	}
	else
	{
		kickCounter++;
		if(kickCounter > 10)
		{
			theGlobalInfo->kickerNum = 0;
			kickCounter =0;
		}
	}
	//theGlobalInfo->kickerNum = theTeamMateInfo->teamMateInfo1.kickPlayer | theTeamMateInfo->teamMateInfo2.kickPlayer | theTeamMateInfo->teamMateInfo3.kickPlayer | theTeamMateInfo->teamMateInfo4.kickPlayer;

	if (theTeamMateInfo->teamMateInfo1.kickPlayer == 1)
	{
		theGlobalInfo->kickerNum = 1;
	}


	theGlobalInfo->shadowLoc.clear();
	genBallDis(theTeamMateInfo,theGlobalInfo);
	genBallLoc(theTeamMateInfo,theGlobalInfo->ballLoc);
	if(theGlobalInfo->ballLoc.x != 0)
	{
		theGlobalInfo->lastBallLoc = theGlobalInfo->ballLoc;
	}
	getStates(theTeamMateInfo,theGlobalInfo);
	getFrames(theTeamMateInfo,theGlobalInfo);
	genShadowLoc(theTeamMateInfo,theGlobalInfo);

	/* for debug */
	//RobotLocation debugLoc;
	//debugLoc.x = 1000;
	//debugLoc.y = 0;
	//debugLoc.validity = 1;
	//theGlobalInfo->shadowLoc.push_back(debugLoc);
	std::cout<<"shadow loc numbers = "<<theGlobalInfo->shadowLoc.size()<<std::endl;
	/* debug end */

	outGlobalInfo();
}
void GlobalInfoProvider::outGlobalInfo()
{
	selfMessageQueue->SetMessageData(idGlobalInfo,idNetworkThread,idRobotThread,
		sizeof(GlobalInfo),(char*)theGlobalInfo);
}
void GlobalInfoProvider::genBallLoc(TeamMateInfo *theTeamMateInfo, BallLocation &loc)
{

    if(theTeamMateInfo->teamMateInfo1.ballSeen&&theTeamMateInfo->teamMateInfo1.robotLocation.validity > 0.75)
	{
		double dis = theTeamMateInfo->teamMateInfo1.ballLocation.distance;
		double ballx = theTeamMateInfo->teamMateInfo1.ballLocation.x;
		double bally = theTeamMateInfo->teamMateInfo1.ballLocation.y;
		double angle = theTeamMateInfo->teamMateInfo1.robotLocation.angle + atan2(bally,ballx);
		ball1.x =  theTeamMateInfo->teamMateInfo1.robotLocation.x + dis*cos(angle);
		ball1.y = theTeamMateInfo->teamMateInfo1.robotLocation.y + dis*sin(angle);

	}
	else
	{
		if(ball1Counter < 10);
		else
		{
			ball1.x = 0;
			ball1.y = 0;
		}

	}

	if(theTeamMateInfo->teamMateInfo2.ballSeen&&theTeamMateInfo->teamMateInfo2.robotLocation.validity > 0.75 )
	{
		double dis = theTeamMateInfo->teamMateInfo2.ballLocation.distance;
		double ballx = theTeamMateInfo->teamMateInfo2.ballLocation.x;
		double bally = theTeamMateInfo->teamMateInfo2.ballLocation.y;
		double angle = theTeamMateInfo->teamMateInfo2.robotLocation.angle + atan2(bally,ballx);
		ball2.x = theTeamMateInfo->teamMateInfo2.robotLocation.x + dis*cos(angle);
		ball2.y = theTeamMateInfo->teamMateInfo2.robotLocation.y + dis*sin(angle);
	}
	else
	{
		if(ball2Counter < 10);
		else
		{
			ball2.x = 0;
			ball2.y = 0;
		}

	}

	if(theTeamMateInfo->teamMateInfo3.ballSeen&&theTeamMateInfo->teamMateInfo3.robotLocation.validity > 0.75 )
	{
		double dis = theTeamMateInfo->teamMateInfo3.ballLocation.distance;
		double ballx = theTeamMateInfo->teamMateInfo3.ballLocation.x;
		double bally = theTeamMateInfo->teamMateInfo3.ballLocation.y;
		double angle = theTeamMateInfo->teamMateInfo3.robotLocation.angle + atan2(bally,ballx);
		ball3.x = theTeamMateInfo->teamMateInfo3.robotLocation.x + dis*cos(angle);
		ball3.y = theTeamMateInfo->teamMateInfo3.robotLocation.y + dis*sin(angle);
	}
	else
	{
		if(ball3Counter < 10);
		else
		{
			ball3.x = 0;
			ball3.y = 0;
		}

	}

	if(theTeamMateInfo->teamMateInfo4.ballSeen&&theTeamMateInfo->teamMateInfo4.robotLocation.validity > 0.75 )
	{
		double dis = theTeamMateInfo->teamMateInfo4.ballLocation.distance;
		double ballx = theTeamMateInfo->teamMateInfo4.ballLocation.x;
		double bally = theTeamMateInfo->teamMateInfo4.ballLocation.y;
		double angle = theTeamMateInfo->teamMateInfo4.robotLocation.angle + atan2(bally,ballx);
		ball4.x = theTeamMateInfo->teamMateInfo4.robotLocation.x + dis*cos(angle);
		ball4.y = theTeamMateInfo->teamMateInfo4.robotLocation.y + dis*sin(angle);
	}
	else
	{
		if(ball4Counter < 10);
		else
		{
			ball4.x = 0;
			ball4.y = 0;
		}

	}

	if (ball1.x != 0)
	{
		keeperSeenBallCounter ++;
		if (keeperSeenBallCounter >=3)
		{
			loc = ball1;
		}
	}
	//else if (ball3.x != 0)
	//{
	//	loc = ball3;
	//}
	//else if(ball2.x != 0)
	//{
	//	loc = ball2;
	//}
	else
	{
		keeperSeenBallCounter = 0;
		loc.x = 0;
		loc.y = 0;
	}

	if (abs(loc.y)>2000 || abs(loc.x)>3000)
	{
		loc.x = 0;
		loc.y = 0;
	}

	std::cout<<"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"<<loc.x<<std::endl;
	std::cout<<"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG"<<loc.y<<std::endl;

}

void GlobalInfoProvider::genBallDis(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo)
{
	if(theTeamMateInfo->teamMateInfo1.ballSeen)
	{
		theGlobalInfo->ballDis1 = theTeamMateInfo->teamMateInfo1.ballLocation.distance;
		ball1Counter = 0;
	}
	else
	{
		ball1Counter++;
		if(ball1Counter < 3);
		else
			theGlobalInfo->ballDis1 = 10000;
	}
	if(theTeamMateInfo->teamMateInfo2.ballSeen)
	{
		theGlobalInfo->ballDis2 = theTeamMateInfo->teamMateInfo2.ballLocation.distance;
		std::cout<<theGlobalInfo->ballDis2<<std::endl;
		ball2Counter = 0;
	}
	else
	{
		ball2Counter++;
		if(ball2Counter < 3);
		else
			theGlobalInfo->ballDis2 = 10000;
	}
	if(theTeamMateInfo->teamMateInfo3.ballSeen)
	{
		theGlobalInfo->ballDis3 = theTeamMateInfo->teamMateInfo3.ballLocation.distance;
		ball3Counter = 0;
	}
	else
	{
		ball3Counter++;
		if(ball3Counter < 3 );
		else
			theGlobalInfo->ballDis3 = 10000;
	}
	if(theTeamMateInfo->teamMateInfo4.ballSeen)
	{
		theGlobalInfo->ballDis4 = theTeamMateInfo->teamMateInfo4.ballLocation.distance;
		ball4Counter =0;
	}
	else
	{
		ball4Counter++;
		if(ball4Counter < 3 );
		else
			theGlobalInfo->ballDis4 = 10000;
	}

}
void GlobalInfoProvider::genShadowLoc(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo)
{
	genOneShadowLoc(theTeamMateInfo->teamMateInfo1,theGlobalInfo);
	genOneShadowLoc(theTeamMateInfo->teamMateInfo2,theGlobalInfo);
	genOneShadowLoc(theTeamMateInfo->teamMateInfo3,theGlobalInfo);
	genOneShadowLoc(theTeamMateInfo->teamMateInfo4,theGlobalInfo);
}

void GlobalInfoProvider::calShadowLocPos(RobotLocation ownLoc,RobotLocation seenLoc,RobotLocation& shadowLoc)
{
	double theta = ownLoc.angle;
	shadowLoc.x = ownLoc.x + cos(theta)*seenLoc.x-sin(theta)*seenLoc.y;
	shadowLoc.y = ownLoc.y + sin(theta)*seenLoc.x+cos(theta)*seenLoc.y;
}

void GlobalInfoProvider::genOneShadowLoc(TeamUDPData teamMate,GlobalInfo *theGlobalInfo)
{
	RobotLocation shadowLoc;
	if (teamMate.mateNum == 1)
	{
		shadowLoc.validity = teamMate.locValidity;
		RobotLocation seen1Loc(teamMate.firstMateX,teamMate.firstMateY);
		calShadowLocPos(teamMate.robotLocation,seen1Loc,shadowLoc);
		theGlobalInfo->shadowLoc.push_back(shadowLoc);
	}
	else if (teamMate.mateNum == 2)
	{
		shadowLoc.validity = teamMate.locValidity;
		RobotLocation seen1Loc(teamMate.firstMateX,teamMate.firstMateY);
		RobotLocation seen2Loc(teamMate.secondMateX,teamMate.secondMateY);
		calShadowLocPos(teamMate.robotLocation,seen1Loc,shadowLoc);
		theGlobalInfo->shadowLoc.push_back(shadowLoc);
		calShadowLocPos(teamMate.robotLocation,seen2Loc,shadowLoc);
		theGlobalInfo->shadowLoc.push_back(shadowLoc);
	}
}

void GlobalInfoProvider::getStates(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo)
{
	theGlobalInfo->state1 = theTeamMateInfo->teamMateInfo1.state;
	theGlobalInfo->state2 = theTeamMateInfo->teamMateInfo2.state;
	theGlobalInfo->state3 = theTeamMateInfo->teamMateInfo3.state;
	theGlobalInfo->state4 = theTeamMateInfo->teamMateInfo4.state;
}
void GlobalInfoProvider::getFrames(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo)
{
	theGlobalInfo->frame1 = theTeamMateInfo->teamMateInfo1.frames;
	theGlobalInfo->frame2 = theTeamMateInfo->teamMateInfo2.frames;
	theGlobalInfo->frame3 = theTeamMateInfo->teamMateInfo3.frames;
	theGlobalInfo->frame4 = theTeamMateInfo->teamMateInfo4.frames;
}
MAKE_KS(GlobalInfoProvider)
