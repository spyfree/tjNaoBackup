#ifndef NetworkProvider_H
#define NetworkProvider_H
#include "Module/Representations/KSManager.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/JointData.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "RoboCupGameControlData.h"
#include "GameControlHandle.h"
#include "TeamMateInfo.h"
#include "UdpPoint.h"
DEFINE_KS(NetworkProvider)
//REQUIRES(TeamCfgInfo)
PROVIDES(RoboCupGameControlData)
PROVIDES(TeamMateInfo)
USES(RoboCupGameControlData)
USES(TeamMateInfo)
END_KS

class NetworkProvider :public NetworkProviderBase,BaseProvider
{
public:
	
	NetworkProvider();
	~NetworkProvider();
	void update(RoboCupGameControlData*theRoboCupGameControlData);
	void update(TeamMateInfo *theTeamMateInfo);
	void outRoboCupGameControlData();
	void outTeamMateInfo();
	void inTeamCfgInfo();
	//void rawSwapTeams(RoboCupGameControlData& data);
	bool isThisGame(RoboCupGameControlData& gameData);
	//static void updateNetworkData(RoboCupGameControlData theControlData,TeamMateInfo theNetTeamData);

private:
	//PROCESS_WIDE_STORAGE_STATIC NetworkProvider* theInstance; /**< The only instance of this module. */
	GameControlHandle gameControl;
	//RoboCupGameControlData gameControlData; 
	
	int playerNum;
	int teamNum;
	UdpPoint udpPoint;
	TeamMateInfo teamMateInfoData;
	TeamUDPData *ownTeamInfo;
};
class NetworkManager
{
public:
	 NetworkManager();
	 ~NetworkManager();
	 void initNetworkKs();
	 void destoryNetKs();
	 void executeNetKs();
private:
	void setGlobal();
	// ---- new a self KSManager
	void initKSManager();
	void updateKSManager();
	Blackboard *blackboard;
	SmartPtr<KSManager>theKSManager;

};
#endif
