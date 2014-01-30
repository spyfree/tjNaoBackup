#include "RobotStateProvider.h"
//#include "MatlabHelp/MatlabHelp.h"
RobotStateProvider::RobotStateProvider()
{

}
void RobotStateProvider::update(RobotState* theRobotState)
{
#ifdef NAO_DEBUG
	receiveExternRobotState(theRobotState);
#endif
	//===Calculate ZMP measured from LIPM
	double bRoll = theSafeSensorData->data[SensorData::angleX];
	double bTilt = theSafeSensorData->data[SensorData::angleY];
	Vector3<double> acc(theSafeSensorData->data[SensorData::accX], theSafeSensorData->data[SensorData::accY], theSafeSensorData->data[SensorData::accZ]);
	
	Vector3<double> pCOM(theNaoStructure->pCOM);
	RotationMatrix Rbw = RotationMatrix().rotateX(bRoll).rotateY(bTilt);
	RotationMatrix Rfw;//rotation of supporting foot in world cooridnate
	if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		Rfw = theNaoStructure->uLink[NaoStructure::lFootLink].R * theNaoStructure->uLink[NaoStructure::bodyLink].R.invert();
	}else
	{
		Rfw =  theNaoStructure->uLink[NaoStructure::rFootLink].R * theNaoStructure->uLink[NaoStructure::bodyLink].R.invert();
	}
	Rfw = Rfw * Rbw;
	// TODO: Check direction of com and acc
	Vector3<double> comW = Rfw * pCOM;
	acc = Rbw * acc;
	//acc = rotB2S * acc;
	double zSup = 0;
	theRobotState->mZMP.z = zSup;
	theRobotState->mZMP.x = comW.x - (comW.z - zSup) * acc.x / (acc.z);
	theRobotState->mZMP.y = comW.y - (comW.z - zSup) * acc.y / (acc.z);
	
	//CoP Detector
	copDetector.update(*theSensorData);
	theRobotState->mCoPL= copDetector.getCoP(true);
	theRobotState->mCoPR = copDetector.getCoP(false);
	//Ground Contact Detector
	groundContactDetector.update(*theSensorData, *theActuatorFrameInfo);
	theRobotState->groundContactLeft = groundContactDetector.contactStateLeft;
	theRobotState->groundContactRight = groundContactDetector.contactStateRight;

	/*MATLABHELP_PLOT(acc.x,"accWx");
	MATLABHELP_PLOT(acc.y,"accWy");
	MATLABHELP_PLOT(acc.z,"accWz");
	MATLABHELP_PLOT(bRoll, "bRoll");
	MATLABHELP_PLOT(bTilt, "bTilt");*/
}	

bool RobotStateProvider::receiveExternRobotState(RobotState* theRobotState)
{
	RobotState buf;
	if (selfMessageQueue->SearchMyMessage(idRobotState,idODESimulation, idActuatorThread,
		(char*)&buf, sizeof(RobotState)))
	{
		theRobotState->fallState = buf.fallState;
// 		if(theRobotState->fallState.fall)
// 			std::cout<<"+"<<std::endl;
// 		else
// 			std::cout<<" "<<std::endl;
	}
	return false;
}

MAKE_KS(RobotStateProvider)