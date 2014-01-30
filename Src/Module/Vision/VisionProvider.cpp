#include "VisionProvider.h"
#include <iostream>
////////////////////////////////////
#include "Module/Vision/ObjectToDraw.h"
using namespace std;
VisionProvider::VisionProvider()
{

	
}
VisionProvider::~VisionProvider()
{

}

void VisionProvider::update(FrameInfo* theFrameInfo)
{
	inTJImage();
	inSafeJointData();
	inSafeSensorData();
	ObjectToDraw::getInstance()->setMessageQueue();
	std::cout<<"theFrameInfo"<<theFrameInfo<<std::endl;
}	

void VisionProvider::inTJImage()
{
	selfMessageQueue->SetMessageData(idSimImage,idVisionThread,idImageDialog,
		sizeof(TJImage),(char*)theTJImage);
}
void VisionProvider::inSafeJointData()
{
	selfMessageQueue->SetMessageData(idSensoryJointData,idVisionThread,idImageDialog,
		sizeof(SafeJointData),(char*)(theSafeJointData));

}
void VisionProvider::inSafeSensorData()
{
	selfMessageQueue->SetMessageData(idSimSensorData,idVisionThread,idImageDialog,
		sizeof(SafeSensorData),(char*)(theSafeSensorData));
}
void VisionProvider::outFrameInfo()
{

}
MAKE_KS(VisionProvider)