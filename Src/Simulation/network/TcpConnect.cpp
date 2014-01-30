#include "TcpConnect.h"
TcpConnect ::TcpConnect(QObject *object/* =0 */):QObject(object)
{
	CreatConnect();
}
TcpConnect::~TcpConnect()
{
	//receiveThread.receiveSocket->close();
}
void TcpConnect::DoConnect(ConnectInfo &robotinfo)
{
	receiveThread.StartRun(robotinfo);
	//receiveThread.StartReceive(robotinfo);
	sendThread.startSend(robotinfo);
}
void TcpConnect::SendCommand(NetworkMsg msg)
{
	sendThread.SendCommand(msg);
}

void TcpConnect::CreatConnect()
{
	connect(&receiveThread,SIGNAL(ReceiveOk(char *,const WDataInfo&))
		,this,SIGNAL(ReceiveOk(char *,const WDataInfo& ))/*, Qt::QueuedConnection*/);
	connect(&receiveThread,SIGNAL(ReceiveLocOk(char *,const WDataInfo&)),this,SIGNAL(ReceiveLocOk(char *,const WDataInfo&)));
	//connect(&receiveThread,SIGNAL(ReceiveResultOk(char *))
	//	,this,SIGNAL(ReceiveResultOk(char *))/*, Qt::QueuedConnection*/);
	connect(&receiveThread,SIGNAL(connected()),this,SIGNAL(connected()), Qt::QueuedConnection);
	connect(&receiveThread,SIGNAL(ConnectError()),this,SIGNAL(ConnectError()), Qt::QueuedConnection);
	connect(this,SIGNAL(DoDisconnect()),&receiveThread,SLOT(disConnect()), Qt::QueuedConnection);
	connect(this,SIGNAL(DoDisconnect()),&sendThread,SLOT(disConnect()), Qt::QueuedConnection);
}

ManageTcp::ManageTcp(QObject *object):QObject(object)
{
	IsConnect=false;
	tcpclient=new TcpConnect;
	connect(tcpclient,SIGNAL(connected()),this,SLOT(TcpOk()));
	connect(tcpclient,SIGNAL(ConnectError()),this,SIGNAL(ConnectError()));
	connect(this,SIGNAL(DoDisconnect()),tcpclient,SIGNAL(DoDisconnect()));
	connect(tcpclient,SIGNAL(ReceiveOk(char *,const WDataInfo& )),this,SIGNAL(ReceiveOk(char *,const WDataInfo&)));
 	connect(tcpclient,SIGNAL(ReceiveLocOk(char *,const WDataInfo& )),this,SIGNAL(ReceiveLocOk(char *,const WDataInfo& ))); 
}

ManageTcp::~ManageTcp()
{
		tcpclient=NULL;
}
void ManageTcp::TryConnect(ConnectInfo robotinfo)
{
	connectRobotInfo=robotinfo;
	
	tcpclient->DoConnect(connectRobotInfo);
}
void ManageTcp::SendData(NetworkMsg commandMsg)
{
	if(IsConnect==true)
	 tcpclient->SendCommand(commandMsg);
}
void ManageTcp::Dodiscon()
{
	//delete tcpclient;
	
	//tcpclient=NULL;
	IsConnect=false;

	emit DoDisconnect();
}

void ManageTcp::TcpOk()
{
	IsConnect=true;
	emit ConnectOk();
}

