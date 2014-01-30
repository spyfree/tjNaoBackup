#ifndef MessageQueue_H
#define MessageQueue_H
#include "Thread.h"
#include <list>
#include "Module/Core/CommandMsg.h"
#define MESSAGE_MAXSIZE 100
template<class T>class MessageQueueBase
{

public:
	MessageQueueBase( ){};
	T GetTopValue()
	{
		return valueQueue.front();
	}
	void SetBackValue(T value)
	{
		valueQueue.push_back(value);
	}
	double GetSize()
	{
		return valueQueue.size();
	}
	std::list<CommandMsg>::iterator  GetBegin()
	{
		return valueQueue.begin();
	}
	std::list<CommandMsg>::iterator  GetEnd()
	{
		return valueQueue.end();
	}
	void PopTopValue()
	{
		GetTopValue().releaseData();
		//delete(*GetBegin());
		valueQueue.pop_front();

	}
	void DeleteValue(std::list<CommandMsg>::iterator itr)
	{
		itr->releaseData();
		valueQueue.erase(itr);
	}

	void lockValue()
	{
		valueLock.lock();
	}
	void unlockValue()
	{
		valueLock.unlock();
	}
private:
	std::list<T> valueQueue;
	Mutex valueLock;

};


class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

	static MessageQueue* getInstance();
	void SetMessageData(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread,const long &size,char*data);
	CommandMsg GetTopMessage();
	CommandMsg GetCurMessage(int index,char*data,int size);
	bool IsMyMessage(const MessageType &messageType);
	bool SearchMyMessage(CMsgID dataID,CMsgThID sendThread,CMsgThID receiveThread,char * data,int size=-1);
private:
	static MessageQueue *messageQueue;
	SmartPtr<MessageQueueBase<CommandMsg> > commandMsgQueue;
	//CommandMsg* cMsgData;

};

#endif
