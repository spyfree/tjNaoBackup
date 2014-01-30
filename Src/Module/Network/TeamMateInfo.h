#ifndef TEAMMATEINFO_H
#define TEAMMATEINFO_H
#include "./networkComm/wdatainfo.h"
#include <vector>
class perceiveInfo
{
public:
	int team;
	int player;
	unsigned char color;
	bool  send;
	bool  keeperAlive;
	float reliability;

	BallLocation ballLocation;
	perceiveInfo(){}
};

class TeamMateInfo
{
public:
	TeamUDPData teamMateInfo0;
	TeamUDPData teamMateInfo1;
	TeamUDPData teamMateInfo2;
	TeamUDPData teamMateInfo3;
	TeamUDPData teamMateInfo4;
};
class TeamMatesInfo
{
public:
	std::vector<TeamUDPData> MatesInfo;
	TeamMatesInfo()
	{MatesInfo.reserve(2);}
};

#endif
