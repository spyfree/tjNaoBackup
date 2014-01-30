#include "MessageQueue.h"
MessageQueue *MessageQueue::messageQueue=0;
MessageQueue::MessageQueue()
{
	messageQueue=this;
	commandMsgQueue=SmartPtr<MessageQueueBase<CommandMsg> >(new MessageQueueBase<CommandMsg>);
}
MessageQueue::~MessageQueue()
{
	if(messageQueue!=NULL)
	{

		delete messageQueue;
		MessageQueue::messageQueue=NULL;
	}
	/*if(cMsgData!=NULL)
	{
		delete cMsgData;
		cMsgData = NULL;
	}*/
}

MessageQueue* MessageQueue::getInstance()
{
	if(!messageQueue)
	{
		messageQueue = new MessageQueue();
	}
	return messageQueue;
}

CommandMsg MessageQueue::GetTopMessage()
{
	if(commandMsgQueue->GetSize()>0)
	{
		CommandMsg cMsgData;
		// cMsgData=commandMsgQueue->GetTopValue();
		commandMsgQueue->PopTopValue();
		return cMsgData;
	}
}
CommandMsg MessageQueue::GetCurMessage(int index,char*data,int size)
{
	int i=0;
	CommandMsg cMsgData;
	for(std::list<CommandMsg>::iterator itr = commandMsgQueue->GetBegin(); itr != commandMsgQueue->GetEnd(); ++itr)
	{
		if(i==index)
		{
			//cMsgData=(*itr);
			commandMsgQueue->DeleteValue(itr);
			return cMsgData;
		}
		i++;

	}
	//return NULL;
}

bool MessageQueue::IsMyMessage(const MessageType &messageType)
{
	for(std::list<CommandMsg>::iterator itr = commandMsgQueue->GetBegin(); itr != commandMsgQueue->GetEnd(); ++itr)
	{
		if(itr->IsMessageType(messageType))
		{
			return true;
		}
	}
	return false;
}

bool MessageQueue::SearchMyMessage(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread,char* data,int size)
{
// 	commandMsgQueue->lockValue();
// 	MessageType messageType(dataID,sendThread,receiveThread);
// 
// 	for(std::list<CommandMsg>::iterator itr = commandMsgQueue->GetBegin(); itr != commandMsgQueue->GetEnd(); ++itr)
// 	{
// 		if(itr->IsMessageType(messageType))
// 		{
// 			itr->GetData(data,size);
// 			commandMsgQueue->DeleteValue(itr);
// 			commandMsgQueue->unlockValue();
// 			return  true;
// 
// 		}
// 	}
// 	commandMsgQueue->unlockValue();
// 	return  false;
	commandMsgQueue->lockValue();
	MessageType messageType(dataID,sendThread,receiveThread);
	std::list<CommandMsg>::iterator itr = commandMsgQueue->GetBegin();
	std::list<CommandMsg>::iterator itrtmp = commandMsgQueue->GetBegin();
	bool found_my_message=false;
	while(itr != commandMsgQueue->GetEnd())
	{
		if (itr->IsMessageType(messageType))
		{
			itr->GetData(data,size);
			itrtmp=itr++;
			commandMsgQueue->DeleteValue(itrtmp);
			found_my_message=true;
		}
		else
			itr++;

	}
	commandMsgQueue->unlockValue();

	return  found_my_message;
}

void MessageQueue::SetMessageData(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread,const long &size,char*data )
{
	commandMsgQueue->lockValue();
	 CommandMsg cMsgData ;
	MessageType messageType(dataID,sendThread,receiveThread);
	cMsgData.setValue(messageType,size,data);
	if(commandMsgQueue->GetSize()<MESSAGE_MAXSIZE)
		commandMsgQueue->SetBackValue(cMsgData);
	else
	{
		commandMsgQueue->PopTopValue();
		commandMsgQueue->SetBackValue(cMsgData);
	}
	commandMsgQueue->unlockValue();

}