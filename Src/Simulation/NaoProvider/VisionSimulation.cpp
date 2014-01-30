#include "VisionSimulation.h"
#include "Module/Core/CommandMsg.h"
#include "Platform/GetSetting.h"
#include <iostream>
using namespace std;
VisionSimulation::VisionSimulation()
{


}
VisionSimulation::~VisionSimulation()
{

}
void VisionSimulation::update(TJImage* theTJImage)
{
	selfMessageQueue->SearchMyMessage(idUncorrectedRaw,idImageDialog,idVisionThread,
		(char*)theTJImage->unCorrectedRaw, theTJImage->imageSize);

	selfMessageQueue->SearchMyMessage(idLut,idImageDialog,idVisionThread,
		(char*)&(theTJImage->TJlut), sizeof(theTJImage->TJlut));

	//get image from ode
	if (selfMessageQueue->SearchMyMessage(idSimImage,idODESimulation,idVisionThread,
		(char*)theTJImage->unCorrectedRaw, theTJImage->imageSize))
	{
		std::string path = std::string(GetSetting::CovertRoot()) +"\\TJLut.dat";
		theTJImage->loadLUT(const_cast<char*>(path.c_str()),theTJImage->lut_);
		theTJImage->setLUT(theTJImage->lut_);
		std::cout<<"ode get a pic"<<std::endl;
	}
}
void VisionSimulation::update(SafeJointData* theSafeJointData)
{
	selfMessageQueue->SearchMyMessage(idSensoryJointData,idImageDialog,idVisionThread,
		(char*)theSafeJointData,sizeof(SafeJointData));

	//get sensor joint data from ode
	selfMessageQueue->SearchMyMessage(idSensoryJointData,idActuatorThread, idVisionThread,
		(char*)theSafeJointData,sizeof(SafeJointData));
}
void VisionSimulation::update(SafeSensorData* theSafeSensorData)
{
	selfMessageQueue->SearchMyMessage(idSimSensorData,idImageDialog,idVisionThread,
		(char*)theSafeSensorData,sizeof(SafeSensorData));

	//get sensor data from ode
	selfMessageQueue->SearchMyMessage(idSimSensorData,idActuatorThread, idVisionThread,
		(char*)theSafeSensorData,sizeof(SafeSensorData));

}
void VisionSimulation::update(FrameInfo* theFrameInfo)
{
	selfMessageQueue->SearchMyMessage(idFrameInfo,idODESimulation, idVisionThread,
		(char*)theFrameInfo,sizeof(FrameInfo));
	//std::cout<<"visionsimulation"<<theFrameInfo<<std::endl;
}

void VisionSimulation::outTJImage()
{
// 	curMessageType->setMessageType(idTJImage,idVisionThread,idImageDialog);
// 	curCommandMsg->setValue(*curMessageType,sizeof(TJImage),(char*)(theTJImage));
// 	selfMessageQueue->SetMessageData(*curCommandMsg);
}
void VisionSimulation::outSafeJointData()
{

}
// void VisionSimulation::outVisionInterface()
// {
// 	// Notation: A member provided is private in the base calss, and can not be directly used.
// 	// However, we can use a member pointer to get the variable from update...
// }


//VisionSimuManager::VisionSimuManager()
//{
//    blackboard=new Blackboard;
//}
//VisionSimuManager::~VisionSimuManager()
//{
//	if(blackboard)
//	{
//		delete blackboard;
//		blackboard=NULL;
//	}
//
//}
//void VisionSimuManager::initNetworkKs()
//{
//	setGlobal();
//	initKSManager();
//	updateKSManager();
//}
//void VisionSimuManager::executeNetKs()
//{
//	theKSManager->execute();
//}
//void VisionSimuManager::destoryNetKs()
//{
//	theKSManager->destroy();
//}
//void VisionSimuManager::setGlobal()
//{
//	Blackboard::theInstance=blackboard;
//}
//void VisionSimuManager::initKSManager()
//{
//	theKSManager=new KSManager("SimulationThread");
//}
//void VisionSimuManager::updateKSManager()
//{
//	theKSManager->update(0);
//}



MAKE_KS(VisionSimulation)