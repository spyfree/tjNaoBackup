#ifndef NetWork_H
#define NetWork_H
#include "Module/Core/Thread.h"
#include "Module/Core/MessageQueue.h"
#include "NetworkProvider.h"
//#include "UdpPoint.h"
#include "TcpConnect.h"
#include "GameControlHandle.h"
#include "TeamMateInfo.h"
class NetWork :public Thread
{
public:
	NetWork(SmartPtr<Synchronizer> _synchro);
	~NetWork();
	int start();
	void run();
	void stop();
	void reset();
	int startTcp();
	void stopTcp();
	void send();

private:
	void receive();

	//UdpPoint udpPoint;
	//TeamMateInfo teamMateInfoData;
	//GameControlHandle gameControl;
	TcpConnect tcpConnect;
	//RoboCupGameControlData gamecontrolData;
	NetworkManager networkmanager;


};
#endif
