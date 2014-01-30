#include "sendthread.h"
#include <iostream>
SendThread::SendThread(QObject *parent):QThread(parent)
{
	sendSocket=NULL;
	sendRobot =new ConnectInfo;
	creatconnect();
	isAbort=false;
	isConnect=false;
}
SendThread::~SendThread()
{
	sendSocket->close();
	if(isRunning())
	{
	
		if(!wait(1000))
		{
			terminate();
		}
	}
	if(sendSocket!=NULL)
		delete sendSocket;
	isAbort=false;
	isConnect=false;
	while(CommandQueue.size() > 0)
		CommandQueue.clear();
}
void SendThread::creatconnect()
{
//connect(sendSocket,SIGNAL(connected()),this,SLOT(sendData()));
	connect(this, SIGNAL(QuitSendLoop()), this, SLOT(quit()));
}
void SendThread::startSend(ConnectInfo &robotinfo)
{
	std::cout<<"in SendThread::startSend############ "<<std::endl;
	isAbort=true;
	sendRobot->Copy(robotinfo);
	if(!isRunning())
		start();//调用了 run函数
	else
	{cond.wakeOne();
	std::cout<<"startsend不成功"<<std::endl;
	}
}
void SendThread::run()
{
	std::cout<<"in SendThread:: run ++++++++++++++"<<"isAbort-----"<<isAbort<<std::endl;
	if(sendSocket==NULL)
	{
		std::cout<<"in SendThread::run, to new a sendSocket "<<std::endl; 
		std::cout<<"--isAbort--"<<isAbort<<std::endl;
	  sendSocket=new QTcpSocket();  
	 
	  creatconnect();
	
	}
	//if(!isConnect&&tryNewConnect());
	if(!isConnect)
	{  tryNewConnect();
	std::cout<<"isConnect----"<<isConnect<<"isAbort----"<<isAbort<<std::endl;
	}
	if(isAbort&&isConnect)//sendSocket->state() == QAbstractSocket::ConnectedState)
	{
		std::cout<<"in SendThread::run , begin to sendData"<<std::endl;
		//while(CommandQueue.size() > 0)
		//{
		//	MsgQueueMutex.lock();
		//	NetworkMsg tmpMsg = CommandQueue.front();
		//	CommandQueue.pop_front();
		//	MsgQueueMutex.unlock();
			sendData( );
		//}
	}
	else 
	{
		return ;
	}
	//exec();
	//sendSocket->disconnectFromHost();
}

/*sendData()发送的只有一个文件头，没有实际的信息内容*/
void SendThread::sendData(/*NetworkMsg msg*/)//need data and command
{
	//NetworkMsg sendMsg=msg;//=CommandQueue.front();
	//int sendSize=ProduceData::PrepareData(SendDataBuff);//不用PrepareData准备数据，直接写在下面
	while(CommandQueue.size() > 0)
		{
            NetworkMsg tmpMsg = CommandQueue.front();
		    CommandQueue.pop_front();
		      WDataInfo datainfo;
	       strcpy(datainfo.header, TcpHeader);
	       datainfo.robot=2;
	         datainfo.idtype=tmpMsg.RS_COMMAND;
	         datainfo.size=0;
	         datainfo.commandtype = INFO_REQUEST;
	       std::cout<<"datainfo robot is "<<datainfo.robot<<std::endl;
	       memcpy(SendDataBuff,&datainfo,sizeof(WDataInfo));
	//memcpy(SendDataBuff +sizeof(WDataInfo), &testPackage,datainfo.size );


	connect(sendSocket, SIGNAL(disconnected()),
		sendSocket, SLOT(deleteLater()));
	sendSocket->write(SendDataBuff,sizeof(WDataInfo));//SendDataBuff 是实际要发送的包
	std::cout<<"in sendThread::sendData, finish to write SendBuffer "<<std::endl;
	if(!sendSocket->waitForBytesWritten(1000))
	{	
		sendSocket->disconnectFromHost();
		return;
	}
	msleep(100);
	}
}
bool SendThread::tryNewConnect()
{
	std::cout<<"in SendThread::tryNewConnect"<<std::endl;
	//std::cout<<"--isAbort--"<<isAbort<<std::endl;
	sendSocket->abort();

	sendSocket->connectToHost(sendRobot->ipstring,SENDPORT);
	 isConnect=sendSocket->waitForConnected(2000);
	 std::cout<<"in SendThread::tryNewConnect, isConnect is "<<isConnect<<std::endl;
	if(isConnect)
	{	
	
		return true;
	}
	else 
		return false;

}

void SendThread::SendCommand(NetworkMsg msg)
{
	MsgQueueMutex.lock();
	CommandQueue.append(msg);
	MsgQueueMutex.unlock();
	if(!isRunning())
		start();//调用了 run函数
	else
		cond.wakeOne();

}

void SendThread::disConnect()
{
	isAbort=false;
	isConnect=false;
	while(CommandQueue.size() > 0)
		CommandQueue.clear();
	//
	
	//if(sendSocket!=NULL)
	//	delete sendSocket;
	if(isRunning()){
		{
		
			emit QuitSendLoop();
		}
		if(!wait())
		{
			terminate();
		}	

	
	}
}
Instruction getRobotInstruction(const NetMsgID stationCMId)
{
	switch(stationCMId)
	{
	case idUndefined: return NO_COMMAND;
	case idRawImage: return CPLANE_SEND;
	case idWalkType: return  WALKTYPE_CHANGE;
	case idWalkSpeed: return FORWARD_MOVE;
	default: return NO_COMMAND;
	}
}