#include <string.h>
#include <iostream>
#include "CommandMsg.h"
using namespace std;
#define USE_SENDTHREAD_MSG
MessageType::MessageType()
{
	init();
}
MessageType::MessageType(CMsgID dataID,CMsgThID sendThread ,CMsgThID receiveThread )
{
	RS_COMMAND=dataID;

	RecThread=receiveThread;
#ifdef USE_SENDTHREAD_MSG
	SendThread=sendThread;
#endif
}
void MessageType::init()
{
	RS_COMMAND=idUndefine;
	RecThread=idundefineThread;
#ifdef USE_SENDTHREAD_MSG
	SendThread=idundefineThread;
#endif
}



void MessageType::setMessageType(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread)
{

	RS_COMMAND=dataID;

	RecThread=receiveThread;
#ifdef USE_SENDTHREAD_MSG
	SendThread=sendThread;
#endif
}
bool MessageType::operator ==(const MessageType&type)
{
#ifdef USE_SENDTHREAD_MSG
	if(RS_COMMAND==type.RS_COMMAND&&SendThread==type.SendThread&&RecThread==type.RecThread)
		return true;
	else
		return false;
#else
	if(RS_COMMAND==type.RS_COMMAND&&RecThread==type.RecThread)
		return true;
	else
		return false;
#endif
}



CommandMsg ::CommandMsg()
{
	init();
}
CommandMsg::~CommandMsg()
{
	
}
void CommandMsg::releaseData()
{
	if(CmData!=NULL)
	{

		delete []CmData;
		CmData=NULL;

	}
}

CommandMsg::CommandMsg(const MessageType& type,const long &size,char *data)
{
	CmSize=size;
	messageType=type;
	CmData=data;

}
void CommandMsg::init()
{
	messageType.init();
	CmSize=0;
	CmData=NULL;
}
void CommandMsg::setValue(const MessageType &type,const long &size,char*data )
{
	messageType=type;
	CmSize=size;
	CmData = new char[size];
	memcpy(CmData,data,size);
}
void CommandMsg::GetData(char *data,int size)
{
	if( size!=-1)
		memcpy(data,CmData,size);
	else
		memcpy(data,CmData,CmSize);

}

bool CommandMsg::operator ==(const CommandMsg&msg)
{
	if(messageType==msg.messageType&&CmSize==msg.CmSize&&CmData==msg.CmData)
	{
		return true;
	}
	return false;

}

bool CommandMsg::IsMessageType(MessageType isMessageType)
{
	if(messageType==isMessageType)
	{
		return true;
	}
	return false;


}

