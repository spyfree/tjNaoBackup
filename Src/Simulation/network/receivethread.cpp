#include "receivethread.h"
#include <QtNetwork>
#include <iostream>
//Q_REGISTER_METATYPE
const int imagebuf_size = 320*240;
const int netVisionInterf_size = sizeof(netVisionInterface);
const int robotsample_size=sizeof(Robotsamplebuffer);
const int ballsample_size=sizeof(Ballsamplebuffer);
const int freepart_size=sizeof(FreePartData);
ReceiveThread::ReceiveThread(QObject *parent/* =0 */):QThread(parent)
{
	receiveSocket=NULL;
	receiveRobot =new ConnectInfo;
	IsDataArive=false;
	//recbuffer = new char[320*240];
	recbuffer = (char*)malloc(imagebuf_size+netVisionInterf_size+1);
	relocbuffer=(char*)malloc(robotsample_size+ballsample_size+freepart_size+1);
	//relocbuffer=new samplebuffer();
	//visionResult = (char*)malloc(netVisionInterf_size);

	// qRegisterMetaType<ReceiveThread>("ReceiveThread");
}
ReceiveThread::~ReceiveThread()
{

    receiveSocket->close();
	if(isRunning())
	{
		emit QuitRecieverLoop();
		if(!wait(100))
		{
			terminate();
		}
	}
	//if(receiveSocket!=NULL)
	//	delete receiveSocket;
	receiveRobot=NULL;
	if (recbuffer!=NULL)
	{
		delete recbuffer;
		recbuffer = NULL;
	}
	free(relocbuffer);
	/*if(relocbuffer!=NULL)
	{
		delete relocbuffer;
		relocbuffer=NULL;
	}*/
	//if (visionResult!=NULL)
	//{
	//	delete visionResult;
	//	visionResult=NULL;
	//}
}
void ReceiveThread::creatconnect()
{
	//connect(this,SIGNAL(errorConnect()),this,SLOT(diserror()));//没有errorConnect这个信号啊
	connect(this, SIGNAL(QuitRecieverLoop()), this, SLOT(quit()));
	connect(this,SIGNAL(disconnectself()),this,SLOT(disConnect()), Qt::QueuedConnection);
	connect(receiveSocket,SIGNAL (readyRead()),this,SLOT(receiveData()));
	connect(receiveSocket,SIGNAL(error(QAbstractSocket::SocketError )),this,SLOT(diserror()));
}
void ReceiveThread::StartReceive(ConnectInfo &robotinfo)
{

	receiveRobot->Copy(robotinfo);	
	//QMutexLocker locker(&mutex);
	if(!isRunning())
		start();
	else
		cond.wakeOne();
	

}
void ReceiveThread::StartRun(ConnectInfo &robotinfo)
{
	std::cout<<"ReceiveThread::StartRun-------- "<<std::endl;
	receiveRobot->Copy(robotinfo);
    
	if(receiveSocket==NULL)
	{
    std::cout<<"in ReceiveThread::StartRun,new QTcpSocket "<<std::endl;
	receiveSocket = new QTcpSocket;
	 creatconnect();
	}
	
	if(tryNewConnect())
	emit connected();
	else 
	{
		std::cout<<"in ReceiveThread::StartRun,  ConnectError "<<std::endl;
		emit ConnectError();
		return ;
	}	
	//exec();
}

void ReceiveThread::run()
{
	std::cout<<"in ReceiveThread:: run ++++++++++++++"<<std::endl;
	if(receiveSocket==NULL)
	{
	receiveSocket = new QTcpSocket;
	creatconnect();
	}

	if(tryNewConnect())
		emit connected();
	else 
	{
		std::cout<<"in ReceiveThread::run, send ConnectError "<<std::endl;
		emit ConnectError();
		return ;
	}	
	exec();
	
	//receiveSocket->waitForDisconnected();
	//emit receiveData();
}
bool ReceiveThread::tryNewConnect()
{
	std::cout<<"in ReceiveThread::tryNewConnect "<<std::endl;
	receiveSocket->abort();
	receiveSocket->connectToHost(receiveRobot->ipstring,RECEIVEPORT);
	bool Isconnect=receiveSocket->waitForConnected(30000);
	std::cout<<"in ReceiveThread::tryNewConnect, the Isconnect is "<<Isconnect<<std::endl;
	if(Isconnect)
		return true;
	else 
		return false;

}
void ReceiveThread::diserror()
{
	//QMessageBox::warning(parent,tr("waring"),tr("error"),QMessageBox::Yes);
	receiveSocket->close();
}

void ReceiveThread::receiveData()
{
	WDataInfo WTempInfo; 
	//NetMsgID  Messagetype;
	char temp[100]={0};
	while (receiveSocket->bytesAvailable() < sizeof(WDataInfo)) {
		if (!(receiveSocket->waitForReadyRead(4000))) {
			//emit ReciverError(RecieverSocket->error(), RecieverSocket->errorString());
			std::cout<<"in ReceiveThread::receiveData,waitForReadyRead"<<std::endl;
			return;
		}
	}
	//connect(&receiveSocket, SIGNAL(disconnected()),&receiveSocket, SLOT(deleteLater()));
	receiveSocket->read(temp, sizeof(WDataInfo));//读头
	memcpy(&WTempInfo,temp,sizeof(WDataInfo));
	std::cout<<"WTempInfo idtype is "<<WTempInfo.idtype<<"WDataInfo size---"<<WTempInfo.size<<std::endl;
	//Messagetype=WTempInfo.idtype;
	if(strcmp(TcpHeader,WTempInfo.header)!=0)
	{
		emit disconnectself();
		return;
	}
	if(!(receivePackage(WTempInfo) ))
	{
		emit disconnectself();
		return;
	}
	else 
	{
		return ;
	}
	
}
bool ReceiveThread::receivePackage(const WDataInfo& wdatainfo)
{
	bool isSuccess = true;
	//switch(wdatainfo.idtype)
	//{
	//case idClassifidImage:recbuffer=(char*)malloc(imagebuf_size+netVisionInterf_size+1);
	//					  break;
	//case idLocatorData:recbuffer=(char*)malloc(LocatiorData_size+1);
	//					break;
	//}
	//recbuffer = (char*)malloc(wdatainfo.size+1);
	std::cout<<"receive buffer size is "<<wdatainfo.size<<std::endl;//is 6
	int i=receiveSocket->bytesAvailable();
	std::cout<<"receiveSocket->bytesAvailable() -----"<<i<<std::endl;
	while (receiveSocket->bytesAvailable() < wdatainfo.size) {
		if (!(receiveSocket->waitForReadyRead(4000))) {
			isSuccess = false;
			delete recbuffer;
			delete relocbuffer;
			//delete visionResult;
			emit disconnectself();
			return isSuccess;
		}
	}
	//receiveSocket->disconnectFromHost();
	//receiveSocket->waitForDisconnected();
	std::cout<<"in ReceiveThread::receivePackage,receiveSocket begin to read "<<std::endl;
	if(wdatainfo.idtype==idClassifidImage)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	{
		receiveSocket->read(recbuffer,wdatainfo.size);
		recbuffer[wdatainfo.size] = '\0';
		ManagePicData(recbuffer,wdatainfo);
	}
	else if(wdatainfo.idtype==idLocatorData)
	{
	    receiveSocket->read(relocbuffer, wdatainfo.size);
		relocbuffer[wdatainfo.size] = '\0';
		ManageLocData(relocbuffer,wdatainfo);
	}
	//int rectwo=(int)recbuffer[1000];
	//std::cout<<"recbuffer[1000] is "<<rectwo<<std::endl;
	//std::cout<<"recbuffer[2000] is "<<(int)recbuffer[2000]<<std::endl;
	
	int normalSize = 320*240;//208*160*6; 
	double data=0;
	//memcpy(&data,buffer+sizeof(int),8);
	//receiveSocket->disconnectFromHost();
    //memcpy(visionResult,recbuffer+imagebuf_size,netVisionInterf_size);
	//ManageResultData(visionResult);
	//ManagePicData(recbuffer,wdatainfo);

	//std::cout<<"we get the visionResult"<<std::endl;
	IsDataArive=false;
	//delete recbuffer;
	return isSuccess;
}
void ReceiveThread::ManagePicData(char *data,const WDataInfo& wdatainfo)
{
    
	emit ReceiveOk(data, wdatainfo);
	
	//receiveSocket->disconnectFromHost();
	//emit disconnected();
}
void ReceiveThread::ManageLocData(char *data,const WDataInfo&wdatainfo)
{
	emit ReceiveLocOk(data,wdatainfo);
}
//void ReceiveThread::ManageResultData(char *data)
//{
//	//std::cout<<"we are in ManageResultData"<<std::endl;
//	emit ReceiveResultOk(data);
//	//std::cout<<"emit receiveResultdata success"<<std::endl;
//}

void ReceiveThread::disConnect()
{
	
	if(isRunning()){
		{
			
			emit QuitRecieverLoop();
		}
		if(!wait())
		{
			terminate();
		}
	emit disconnected();
}
}
