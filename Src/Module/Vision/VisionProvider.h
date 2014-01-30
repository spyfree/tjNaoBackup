#ifndef VisionProvider_H
#define VisionProvider_H
#include "Module/Representations/BaseProvider.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/FrameInfo.h"
DEFINE_KS(VisionProvider)
REQUIRES(TJImage)
REQUIRES(SafeJointData)
REQUIRES(SafeSensorData)
PROVIDES(FrameInfo)
END_KS

class VisionProvider : public VisionProviderBase,public BaseProvider
{
public:
	VisionProvider();
	~VisionProvider();
/************************************************************************/
/*		Implement functions derived from Base                                        */
/************************************************************************/
	//void update(Image* image);
	void inTJImage();
	void inSafeJointData();
	void inSafeSensorData();
	void update(FrameInfo* theFrameInfo);
	void outFrameInfo();
private:


};


#endif
