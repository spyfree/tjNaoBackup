#include "TcpConnect.h"
#define SEND_PORT
const int imagebuf_size = 320*240;
const int netVisionInterf_size = sizeof(netVisionInterface);
const int robotsample_size=sizeof(Robotsamplebuffer);
const int ballsample_size=sizeof(Ballsamplebuffer);
const int FreePart_size=sizeof(FreePartData);

TcpConnect::TcpConnect(SmartPtr<Synchronizer>_synchro):Thread(_synchro,"TcpThread")
{
 //send_request=NO_REQUEST;
 //sendImagep = NULL;
 sendImagep = (char*)malloc(imagebuf_size+netVisionInterf_size);
 memset(sendImagep,0,imagebuf_size+netVisionInterf_size);
 //sendLocp=new samplebuffer();
 sendLocp=(char*)malloc(robotsample_size+ballsample_size+FreePart_size);
 memset(sendLocp,0,robotsample_size+ballsample_size+FreePart_size);
 sendtobehavior_signaled=false;
}
TcpConnect::~TcpConnect()
{
	free(sendImagep);
	free(sendLocp);
    receivePoint.close();
	sendPoint.close();
}
void TcpConnect::run ()
{
	running = true;
	//std::cout<<"呵呵tcpconnect run**"<<std::endl;
	spMyTrigger->on();
	receivePoint.bind(TCP_RECEIVE_PORT);
#ifdef SEND_PORT
	sendPoint.bind(TCP_SEND_PORT);
#endif
	while (running)
	{
		//cout<<"in TcpConnect::run , begin tcpThread"<<endl;
		receivePoint.accept();
		
#ifdef SEND_PORT
		sendPoint.accept();
#endif
#ifdef DEBUG_TOOL_CONNECTS
		printf("Connection received from the TOOL\n");
#endif
		while ( receivePoint.connected())
		{

   		  receive();//receive request command

		}

	}

	receivePoint.closeAll();
#ifdef SEND_PORT
	sendPoint.closeAll();
#endif


	running = false;
	spMyTrigger->off();
}

void
TcpConnect::reset ()
{
	receivePoint.close();
#ifdef SEND_PORT
	sendPoint.close();
#endif

}

void TcpConnect::receive ()
{
	//std::cout<<"in TcpConnect::receive ------------"<<"sendtobehavior_signaled-------"<<sendtobehavior_signaled<<std::endl;
	WDataInfo WTempInfo;
	char temp[100]={0};
	WDataInfo datainfo;

	receivePoint.read(temp, sizeof(WDataInfo));
	memcpy(&WTempInfo,temp,sizeof(WDataInfo));
	printf("%d---%d---%s\n",WTempInfo.size,WTempInfo.idtype,WTempInfo.header);
	if(strcmp(TcpHeader,WTempInfo.header)==0)
	{
		if(strcmp(TcpHeader,WTempInfo.header)==0)
		{
		char *writebuffer="a";
		cout<<"write"<<endl;
#ifdef SEND_PORT
		//sendPoint.write(writebuffer,sizeof(char)*2);
#endif
		}

		//if(strcmp(TcpHeader,WTempInfo.header)!=0){
		//	 reset();
		//	return;
		//}
		if (WTempInfo.commandtype==INFO_PROVIDE)
		{
			if( !(receivePackage(WTempInfo) ))
			{
				reset();
				return;
			}
			else
			{
				cout<<"receive finish"<<endl;
				return ;
			}
		}
		if (WTempInfo.commandtype==INFO_REQUEST)
		{
			strcpy(datainfo.header, TcpHeader);//准备发送的数据
            datainfo.robot = WTempInfo.robot;
			datainfo.commandtype = INFO_PROVIDE;
			datainfo.idtype = WTempInfo.idtype;
			sendtobehavior_signaled=false;
            handle_request(datainfo);
		}

		if (WTempInfo.commandtype==DISCONNECT)
		{
			reset();
			return;
		}

	}
	else
	{
		cout<<"error header"<<endl;
		reset();
		return;

	}
	switch(datainfo.idtype)
	{
	case idClassifidImage:receive_image(datainfo);
		                  break;
	case idLocatorData:receive_LocData(datainfo);
		                  break;
	default:break;
	}


}

//未完成
bool TcpConnect::receivePackage(const WDataInfo& wdatainfo)
{
	bool isSuccess = true;
	char* buffer = (char*)malloc(wdatainfo.size+1);
	receivePoint.read(buffer, wdatainfo.size);//读取信号的实际内容
	std::cout<<"receivePackage!!!!!!!!!!!!!!!"<<std::endl;
    switch(wdatainfo.idtype)
	{

	default:
		{

			buffer[wdatainfo.size] = '\0';
		}
	}
	//delete buffer;
	free(buffer);
	return isSuccess;

}
void TcpConnect::ManageBuffer(char *buffer,const WDataInfo& wdatainfo)
{

}
//const int imagebuf_size=320*240;
void TcpConnect::handle_request (WDataInfo& wdatainfo)
{
	//hear must add the traslate of instrustion
	//char cinstruction[NAME_SIZE]="d";

	if (!sendtobehavior_signaled&&wdatainfo.idtype == idClassifidImage)
	{
		sendtobehavior_signaled=true;
		
		//TODO:just write one command for image,if it want to send more cmd to behavior such as walktype ,
		//should change the structure of CommandData or send the cmd to one buffer and write it to the messagequeue(using the interrupt in behavior module to handle the cmd)
		cmd.RC_COMMAND=CPLANE_SEND;
		selfMessageQueue->SetMessageData(idVisionCmd,idNetworkThread,idRobotThread,sizeof(CommandData),(char*)&cmd);
		/**************************************************************************************************/
	}
	else if(!sendtobehavior_signaled&&wdatainfo.idtype==idLocatorData)
	{
		sendtobehavior_signaled=true;

		cmd.RC_COMMAND=Loc_Send;
		selfMessageQueue->SetMessageData(idLocCmd,idNetworkThread,idRobotThread,sizeof(CommandData),(char*)&cmd);
	}
   // std::cout<<"handle_request后-----------------sendtobehavior_signaled-------------------"<<sendtobehavior_signaled<<std::endl;

}
void TcpConnect::receive_image(WDataInfo& wdatainfo)
{
	if (sendtobehavior_signaled&&
		selfMessageQueue->SearchMyMessage(idClassifyImage,idRobotThread,idNetworkThread,	(char*)sendImagep,imagebuf_size)&&
		selfMessageQueue->SearchMyMessage(idVisionPercept,idRobotThread,idNetworkThread,(char*)sendImagep+imagebuf_size,sizeof(netVisionInterface)))
	{
		wdatainfo.size = imagebuf_size+netVisionInterf_size;//size应该是wdatainfo后面的信息的长度
		int sendbuffersize = sizeof(WDataInfo)+wdatainfo.size;
		char* sendbuffer = (char*)malloc(sendbuffersize);
		memcpy(sendbuffer,&wdatainfo,sizeof(WDataInfo));//加头
		memcpy(sendbuffer +sizeof(WDataInfo), sendImagep,wdatainfo.size );//加数据
		sendPoint.write(sendbuffer,sendbuffersize);//sendbuffer是真正tcp发送的数据包
		free(sendbuffer);
		sendtobehavior_signaled=false;
		//std::cout<<"receive_image 后sendtobehavior_signaled-------------------"<<sendtobehavior_signaled<<std::endl;

	}
}
void TcpConnect::receive_LocData(WDataInfo& wdatainfo)
{
	if (sendtobehavior_signaled&&
		selfMessageQueue->SearchMyMessage(idLocSample,idRobotThread,idNetworkThread,(char*)sendLocp)&&
		selfMessageQueue->SearchMyMessage(idBallSample,idRobotThread,idNetworkThread,(char*)sendLocp+robotsample_size)&&
		selfMessageQueue->SearchMyMessage(idFreePart,idRobotThread,idNetworkThread,(char*)sendLocp+robotsample_size+ballsample_size))
	{
		
		wdatainfo.size = robotsample_size+ballsample_size+FreePart_size;//size应该是wdatainfo后面的信息的长度
		//wdatainfo.size=sizeof(*sendLocp);
		//std::cout<<"数据包大小size of sndLocp---"<<wdatainfo.size<<std::endl;
		int sendbuffersize = sizeof(WDataInfo)+wdatainfo.size;
		char* sendbuffer = (char*)malloc(sendbuffersize);
		memcpy(sendbuffer,&wdatainfo,sizeof(WDataInfo));//加头
		memcpy(sendbuffer +sizeof(WDataInfo), sendLocp,wdatainfo.size );//加数据
		sendPoint.write(sendbuffer,sendbuffersize);//sendbuffer是真正tcp发送的数据包
		free(sendbuffer);
		sendtobehavior_signaled=false;
		//std::cout<<"-------------------------sendtobehavior_signaled=false;"<<std::endl;

	}
}

void TcpConnect::handle_command (int cmd)
{
	cout<<"handle_command"<<endl;
	switch (cmd) {
	case CMD_TABLE:
		break;

	case CMD_MOTION:
		break;

	case CMD_HEAD:
		break;

	case CMD_JOINTS:
		break;

	default:
		fprintf(stderr, "Unimplemented command type");
	}
}

