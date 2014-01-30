#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include "Module/Network/networkComm/WdataInfo.h"
//#include "NetCommandMsg.h"
#include "Module/Network/networkComm/NetworkMsg.h"
//#include "ProduceData.h"
#include "Tcpconfig.h"
#include "ConnectInfo.h"
#include <QtNetwork>
#include <QTcpSocket>
//#include "NetCommandMsg.h"
#include "Module/Network/networkComm/NetworkMsg.h"
#include "Module/Network/networkComm/WdataInfo.h"
//static const unsigned int NAME_SIZE = 72;
//static const unsigned int VALUE_SIZE = 300;
static const int SENDER_BUFF_SIZE =1024;
class SendThread:public QThread
{
	Q_OBJECT
public:
	SendThread(QObject *parent=0);
	~SendThread();
	void startSend(ConnectInfo &robotinfo);
	void SendCommand(NetworkMsg msg);
signals:
	void QuitSendLoop();
	void disconnected();
private slots:
		void run();
		void sendData(/*NetworkMsg msg*/);
		void disConnect();
private:
	QMutex MsgQueueMutex;
	QTcpSocket *sendSocket;
	WDataInfo datainfo;
	void creatconnect();
	QWaitCondition cond;
	bool tryNewConnect();
	void produceData();
	char SendDataBuff[SENDER_BUFF_SIZE];
	QList<NetworkMsg>CommandQueue;
	ConnectInfo* sendRobot;
	bool isAbort;
	bool isConnect;
};


#endif
