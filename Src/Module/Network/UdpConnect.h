#ifndef UDPCONNECT_H
#define UDPCONNECT_H
#include "UdpPoint.h"
class UdpConnect
{
public:
	UdpConnect();
	~UdpConnect();
	void run();
	void reset();
private:
	void receive();
	UdpPoint udpPoint;
};
#endif