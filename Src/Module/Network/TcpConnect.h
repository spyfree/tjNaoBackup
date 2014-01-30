#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include "networkComm/NetworkMsg.h"
#include "networkComm/WdataInfo.h"
#include "Module/Common/SmartPtr.h"
//#include "Module/Common/VisionHeader.h"
#include "Module/Core/Thread.h"
#include "TcpPoint.h"
#include "alproxy.h"
#include "alptr.h"
#include "altexttospeechproxy.h"
#include "alvisiondefinitions.h"
//#include "alimage.h"
#include "almotionproxy.h"
#include "alvalue.h"
#include "dcmproxy.h"
//#include "Module/Vision/TJImage.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#define DEBUG_TOOL_REQUESTS 1

using namespace AL;


class TcpConnect:public Thread,BaseProvider
{
public:
	TcpConnect(SmartPtr<Synchronizer> _synchro);
	~TcpConnect();
	string getRobotName();
	int getState() { return state; }

private:
	void reset();
	void receive();
	void run();
	void ManageBuffer(char *buffer,const WDataInfo& wdatainfo);
	bool receivePackage(const WDataInfo & wdatainfo);
	void handle_request(WDataInfo& wdatainfo);
	void handle_command(int cmd);
// 	void inTJImage();
	void receive_image(WDataInfo& wdatainfo);
	void receive_LocData(WDataInfo& wdatainfo);
	//SEND_REQUEST send_request;
	int state;
	TcpPoint receivePoint;
	TcpPoint sendPoint;
	//TcpPoint serial;
	bool sendtobehavior_signaled;
	CommandData cmd;
public:
    char *sendImagep;
	char *sendLocp;



};
#endif
