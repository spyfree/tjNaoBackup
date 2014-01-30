#ifndef BaseProvider_H
#define BaseProvider_H

#include "Module/Representations/KS.h"
#include "Module/Representations/KSManager.h"
#include "Module/Common/SmartPtr.h"
#include "Module/Core/MessageQueue.h"
#include "Tools/Math/Common.h"
class BaseProvider
{
public:
	BaseProvider()
	{
		selfMessageQueue=MessageQueue::getInstance();
	}
	~BaseProvider()
	{

	}
protected:
	MessageQueue* selfMessageQueue;
};
#endif
