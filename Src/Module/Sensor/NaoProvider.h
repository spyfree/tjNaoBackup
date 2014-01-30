#ifndef NaoProvider_H
#define NaoProvider_H

#include "Module/Common/FrameInfo.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "alptr.h"
#include "albroker.h"
#include "almodule.h"
#include "alproxy.h"
#include "almemoryfastaccess.h"
// #include "dcmproxy.h"

DEFINE_KS(NaoProvider)
PROVIDES(SafeJointData)
PROVIDES(SafeSensorData)
END_KS


namespace AL
{
	class ALProxy;
	class ALValue;
	//class ALPtr;
};

using namespace AL;

class NaoProvider: public NaoProviderBase,BaseProvider
{
public:
	NaoProvider();
	~NaoProvider();
	void update(SafeJointData* theSafeJointData);
	void update(SafeSensorData *theSafeSensorData);
	void outSafeJointData(){}
	void outSafeSensorData(){}
private:
	//------------use for sensor -----------//
	ALPtr<ALMemoryFastAccess> alfastaccessjoints;
	ALPtr<ALMemoryFastAccess> alfastaccessSensors;
	SensorData sensorData;
	SensoryJointData sensoryJointData;
	std::vector<float> jointangles;
	std::vector<float> sensorValues;
	void registerFastAcess();
	void initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker);
	void getSensor(SafeJointData*inSafeJointData);

	//char *classifiedPic;//[IMAGE_HEIGHT*IMAGE_WIDTH];
	unsigned timeS;

	//////////////////////////////////////////
};
#endif
