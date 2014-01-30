#ifndef VisionSimulation_H
#define VisionSimulation_H
#include "Module/Representations/BaseProvider.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/JointData.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Common/SensorData.h"
DEFINE_KS(VisionSimulation)
PROVIDES(TJImage)
PROVIDES(FrameInfo)
PROVIDES(SafeJointData)
PROVIDES(SafeSensorData)
USES(TJImage)
END_KS

class VisionSimulation : public VisionSimulationBase,BaseProvider
{
public:
	VisionSimulation();
	~VisionSimulation();
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
	//void update(Image* image);
	void update(TJImage* theTJImage);
	void update(FrameInfo* theFrameInfo);
	void update(SafeJointData* theSafeJointData);
	void update(SafeSensorData* theSafeSensorData);
	void outTJImage();
	void outSafeJointData();
	void outFrameInfo(){};
	void outSafeSensorData(){};
};
//class VisionSimuManager
//{
//public:
//	VisionSimuManager();
//	~VisionSimuManager();
//	void initNetworkKs();
//	void destoryNetKs();
//	void executeNetKs();
//private:	
//	void setGlobal();
//	// ---- new a self KSManager
//	void initKSManager();
//	void updateKSManager();
//	Blackboard* blackboard;
//	KSManager* theKSManager;
//
//};




#endif