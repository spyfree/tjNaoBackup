#ifndef GLOBALINFOPROVIDER_H
#define GLOBALINFOPROVIDER_H

#include "Module/Representations/KSManager.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Network/networkComm/wdatainfo.h"
#include "Module/Network/TeamMateInfo.h"



DEFINE_KS(GlobalInfoProvider)
REQUIRES(TeamMateInfo)
PROVIDES(GlobalInfo)
USES(GlobalInfo)
END_KS

class GlobalInfoProvider :public GlobalInfoProviderBase,BaseProvider
{
public:

	GlobalInfoProvider();
	~GlobalInfoProvider();



	void inTeamMateInfo();
	void update(GlobalInfo *theGlobalInfo);

	void outGlobalInfo();


private:
	void genBallLoc(TeamMateInfo *theTeamMateInfo,BallLocation& ballLoc);
	void genBallDis(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo);
	void genShadowLoc(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo);

	void calShadowLocPos(RobotLocation ownLoc,RobotLocation seenLoc,RobotLocation& shadowLoc);
	void genOneShadowLoc(TeamUDPData teamMate,GlobalInfo *theGlobalInfo);

	void getStates(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo);
	void getFrames(TeamMateInfo *theTeamMateInfo,GlobalInfo *theGlobalInfo);

	int ball1Counter;
	int ball2Counter;
	int ball3Counter;
	int ball4Counter;

	BallLocation ball1;
	BallLocation ball2;
	BallLocation ball3;
	BallLocation ball4;

	int kickCounter;
	int keeperSeenBallCounter;



};

#endif