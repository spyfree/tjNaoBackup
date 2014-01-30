#ifndef  WDATAINFO_H
#define WDATAINFO_H
#include "Module/Network/networkComm/NetworkMsg.h"
#include <vector>

const char TcpHeader[4]="TJD";
const char PACKET_HEADER[7]="TJ_UDP";
const char DEBUG_HEADER[7]="TJ_DBG";
#ifndef NUM_PLAYERS_PER_TEAM
# define NUM_PLAYERS_PER_TEAM 4
#endif
static const int MAX_MESSAGE_MEMORY = 30;


enum{KEEPER=1,FORWORD};
class WDataInfo {
public:
	char header[4];
	int  robot;
	NetMsgID idtype;
	int  size;
	CommandType commandtype;
};



typedef struct CommPacketHeader_t
{
	char header[7];
	int team;
	int player;
	int color;
} CommPacketHeader;

class BallLocation
{
public:
	BallLocation():x(0),y(0),cellX(0),cellY(0),distance(0){}
	double x;
	double y;
	double cellX;
	double cellY;
	double distance;

	const BallLocation& operator += (const BallLocation& other)
	{
		x += other.y;
		y += other.y;
		return *this;
	}
	BallLocation operator+(const BallLocation& other) const
	{
		return BallLocation(*this) += other;
	}


};
//robotLocation in world 
class RobotLocation
{
public:
	RobotLocation():x(0),y(0),angle(0),validity(0){}
	double x;
	double y;
	double angle;
	double validity;

	RobotLocation(double locX,double locY)
	{
		x = locX;
		y = locY;
	}

	const RobotLocation& operator=(const RobotLocation& other)
	{
		x = other.x;
		y = other.y;
		angle = other.angle;
		validity = other.validity;
		return *this;
	}
};
class TeamUDPData
{
public:
	TeamUDPData():send(false),state(0),role(5),received(false),ballSeen(false){}
	CommPacketHeader   header;
	bool  send;
	bool received;
	int frames;
	bool ballSeen;
	BallLocation ballLocation;
	RobotLocation robotLocation;
	int state;
	long role;
	int kickPlayer;

	double locValidity;

	int mateNum;
	int firstMateX;
	int firstMateY;
	int secondMateX;
	int secondMateY;
	int mateNear;
};

class GlobalInfo
{
public:
	BallLocation ballLoc;
	BallLocation lastBallLoc;

	RobotLocation keeperLoc;
	RobotLocation attack1Loc;
	RobotLocation attack2Loc;
	RobotLocation defenderLoc;

	std::vector<RobotLocation> shadowLoc;


	double ballDis1;
	double ballDis2;
	double ballDis3;
	double ballDis4;
	int kickerNum;

	int state1;
	int state2;
	int state3;
	int state4;

	int frame1;
	int frame2;
	int frame3;
	int frame4;

	GlobalInfo():state1(0),state2(0),state3(0),state4(0),frame1(0),frame2(0),frame3(0),frame4(0)
	{
		kickerNum = 0;
		ballDis1 = 10000;
		ballDis2 = 10000;
		ballDis3 = 10000;
		ballDis4 = 10000;
		shadowLoc.reserve(8);
	}

};




#endif
