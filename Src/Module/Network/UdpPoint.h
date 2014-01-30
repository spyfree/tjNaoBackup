#ifndef UDPPOINT_H
#define UDPPOINT_H
#include "Module/Common/CommonData.h"
#include "networkComm/NetworkInclude.h"
#include "networkComm/WdataInfo.h"
#include "WirelessConfig.h"
#include "TeamMateInfo.h"
#include <fcntl.h>
#include <list>
#define  UDP_BROADCAST "226.0.22.1"
class UdpPoint
{
public:
	UdpPoint();
	~UdpPoint();
	void bind();
//	void read(TeamMateInfo &data);
	bool readdata(TeamMateInfo &data);
	void write(char *buffer ,int len);
	int select();
	bool insert();
	void close();
	void setOwnTeamInfo(TeamUDPData *ownTeam);
	void send();
	void sendTest();
//	void handleInfo(const char *msg, int len);
	int validatePacket(const char* msg, int len);
	void parsePacket(const char* data);
private:
	int sockn;
	//buffer to receive UDP
	char *buf;
	//my info to send
	//vector<float> ownData;
	int gcTeam;
	int gcColor;

	//to store the teammate info
	std::list<TeamUDPData> latest;
 
	int gotKeeperInfo;
	int gotMateInfo;

	TeamUDPData ownTeamInfo;

	fd_set fdread;
	struct ip_mreq mreq;
	struct sockaddr_in  bind_addr;
	struct sockaddr_in  broadcast_addr;
};


#endif
