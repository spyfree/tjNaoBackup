#ifndef RECEIVETHREAD_H
#define  RECEIVETHREAD_H
#include <QtGui>
#include<QTcpSocket>
#include <QHostAddress>
#include <QMetaType>
#include "Module/Network/networkComm/WdataInfo.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Tools/DrawDebugging/LocMessageData.h"
#include "Tcpconfig.h"
#include "ConnectInfo.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"

class ReceiveThread :public QThread
{
	Q_OBJECT
public:
	ReceiveThread(QObject *parent=0);
	~ReceiveThread();
	void StartReceive(ConnectInfo &robotinfo);
	void StartRun(ConnectInfo &robotinfo);
//	QMutex mutex;

private:
	void run();
	void creatconnect();

signals:
	void ConnectError();
	void connected();
	void disconnectself();
	void QuitRecieverLoop();
	void disconnected();
	void ReceiveOk(char *buffer,const WDataInfo& wdatainfo);
	void ReceiveLocOk(char*buffer,const WDataInfo&wdatainfo);
    //void ReceiveResultOk(char *buffer);
public slots:
	void disConnect();
private slots:
	void receiveData();
	void diserror();

private:
	bool IsDataArive;

	QWaitCondition cond;
	bool receivePackage(const WDataInfo & wdatainfo);
	bool tryNewConnect();
	ConnectInfo *receiveRobot;
	QTcpSocket *receiveSocket;
	void ManagePicData(char *data,const WDataInfo& wdatainfo);
	void ManageLocData(char *data,const WDataInfo& wdatainfo);
	//void ManageResultData(char *data);
public:
	char* recbuffer;
	char* relocbuffer;
    //char* visionResult;
};

#endif


