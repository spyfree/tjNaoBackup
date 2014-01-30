#include "Network.h"
#include "Platform/SystemCall.h"
#include <iostream>
using namespace std;
NetWork::NetWork(SmartPtr<Synchronizer> _synchro):Thread(_synchro,"Network"),tcpConnect(_synchro)
{

}
NetWork::~NetWork()
{
	//udpPoint.close();
	//gameControl.close();

}
//------------function to control the thread----------//
int NetWork::start()
{
	startTcp();
	return Thread::start();
}
int NetWork::startTcp()
{
	//tcpConnect.sendImagep=(char*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH);
	//tcpConnect.sendImagep=NetworkProvider::getclassifiedPic();
	const int result = tcpConnect.start();//调用了run函数,开启了Tcp连接
	if (result > 0)
		// if > 0 -> an error occurred
		// if -1 -> TOOLConnect is already started -> not an error
		fprintf(stderr, "Could not start tcpConnect thread.\n");
	return result;
}
//void NetworkProvider::inTJImage()
//{
//	MessageType type(idTJImage,idRobotThread,idNetworkThread);
//	if (selfMessageQueue->IsMyMessage(type))
//	{
//		memcpy(theTJImageData,selfMessageQueue->SearchMyMessage(type).GetData(), sizeof(TJImage));
//	}
//
//}
void NetWork::stopTcp()
{
	tcpConnect.stop();
}
void NetWork::stop()
{
 stopTcp();
 Thread::stop();
}
static const unsigned NetWork_msPF = 300;

void NetWork::run()
{
	//if you need some code before other thread add here
	std::cout<<"++++++++++++++++ In NetWork::run, start network thread ++++++++++++++"<<std::endl;
	networkmanager.initNetworkKs();
	running=true;
	spMyTrigger->on();

    
	while(running)
	{	
        unsigned startTime = SystemCall::getCurrentSystemTime();
		SystemCall::sleep(100);
		networkmanager.executeNetKs();
		unsigned timeUsed = SystemCall::getTimeSince(startTime);
		std::cout<<"========================Time Used per Frame(NetWork):"<<timeUsed<<" ms "<<std::endl;
		//if (timeUsed < NetWork_msPF)
		//{
		//	SystemCall::sleep(NetWork_msPF-timeUsed);
		//}
	}
	running=false;
	networkmanager.destoryNetKs();
	spMyTrigger->off();
}

void NetWork::send()
{

}
