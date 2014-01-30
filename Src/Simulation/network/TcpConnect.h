#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include "sendthread.h"
#include "receivethread.h"
class TcpConnect:public QObject
{
	Q_OBJECT
public:
	TcpConnect(QObject *object=0);
	~TcpConnect();
		void DoConnect(ConnectInfo &robotinfo);
		void CreatConnect();
	void SendCommand(NetworkMsg msg);
signals:
	void DoDisconnect();
	void ReceiveOk(char *buffer,const WDataInfo& wdatainfo);
	void ReceiveLocOk(char*buffer,const WDataInfo&wdatainfo);
	//void ReceiveResultOk(char *buffer);
	void connected();
	void ConnectError();
private:
	ReceiveThread receiveThread;
	SendThread  sendThread;
};

class ManageTcp:public QObject
{
	Q_OBJECT 
public:
	ManageTcp(QObject *object=0);
	~ManageTcp();
	void Dodiscon();
signals:
	void ConnectOk();
	void ReceiveOk(char *buffer,const WDataInfo& wdatainfo);
	void ReceiveLocOk(char*buffer,const WDataInfo&wdatainfo);
	//void ReceiveResultOk(char *buffer);
	void ConnectError();
	void DoDisconnect();
public slots:
	void TryConnect(ConnectInfo robotinfo);
	void SendData(NetworkMsg commandMsg);
	void TcpOk();
private:
	bool IsConnect;	
	ConnectInfo connectRobotInfo;
public:
	TcpConnect*tcpclient;
};



#endif