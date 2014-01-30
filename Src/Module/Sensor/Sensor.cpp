#include "Sensor.h"
#include "Module/Common/Kinematics.h"
#include <iostream>
#include <fstream>
// TODO: Optimize this function later
void Sensor::update(NaoStructure* theNaoStructure)
{
	inSafeJointData();
	inSafeSensorData();
	theNaoStructure->setLinksJoints(*theSafeJointData);
	//theNaoStructure->setLinksJoints(*theJointCmd);
	theNaoStructure->uLink[NaoStructure::bodyLink].p = Vector3<double>(0,0,0);
	theNaoStructure->uLink[NaoStructure::bodyLink].R = RotationMatrix();
	Kinematics::execForwardKinematics(*theNaoStructure, NaoStructure::bodyLink);
	RotationMatrix bodyRotate(0,theSafeSensorData->data[SensorData::angleY],theSafeSensorData->data[SensorData::angleX]);
	Vector3<double> transL,transR;
	transL = bodyRotate*theNaoStructure->uLink[NaoStructure::lFootLink].p;
	transR = bodyRotate*theNaoStructure->uLink[NaoStructure::rFootLink].p;
	theNaoStructure->supportingMode = transL.z<=transR.z?NaoConfig::SUPPORT_MODE_LEFT:NaoConfig::SUPPORT_MODE_RIGHT;
	/*std::string path = "/opt/naoqi/lib/naoqi/SensorData.txt";
	std::ofstream outFile(path.c_str(),std::ios::app);
	outFile<<"#Sensor#  "<<theNaoStructure->supportingMode<<"     "<<theSafeSensorData->data[SensorData::fsrLFL]<<"   "<<theSafeSensorData->data[SensorData::fsrLFR]<<
		"   "<<theSafeSensorData->data[SensorData::fsrLBL]<<"   "<<theSafeSensorData->data[SensorData::fsrLBR]<<"        "
		<<theSafeSensorData->data[SensorData::fsrRFL]<<"   "<<theSafeSensorData->data[SensorData::fsrRFR]<<"   "<<theSafeSensorData->data[SensorData::fsrRBL]
	<<"   "<<theSafeSensorData->data[SensorData::fsrRBR]<<"   "<<transL.z<<"   "<<transR.z<<"\n";
	outFile.close();*/
	
	Kinematics::recalcBodyPos2SupFoot(*theNaoStructure, Vector3<double>(0,0,0), theNaoStructure->supportingMode);
	Kinematics::execForwardKinematics(*theNaoStructure, NaoStructure::bodyLink);
	theNaoStructure->pCOM = Kinematics::calcCOM(*theNaoStructure);
	//====set bodyangles====
	(*theNaoStructure).setBodyAngles(Vector3<double>(theSafeSensorData->data[SensorData::angleX], theSafeSensorData->data[SensorData::angleY], 0));
	//In case we get NAN or IND num.... print some thing and halt execution when run on robot!!
	ASSERT(theNaoStructure->pCOM == theNaoStructure->pCOM);

	outNaoStructure();
}
void Sensor::inSafeJointData()
{
}
void Sensor::outNaoStructure()
{
}
void Sensor::update(FallDownState* theFallDownState)
{
  //Buffer data:
  buffers[accX].add(theSafeSensorData->data[SensorData::accX]);
  buffers[accY].add(theSafeSensorData->data[SensorData::accY]);
  buffers[accZ].add(theSafeSensorData->data[SensorData::accZ]);

  //Set default state:
  theFallDownState->state = FallDownState::upright;

  //Compute average acceleration values and angles:
  double accXaverage(buffers[accX].getAverage());
  double accYaverage(buffers[accY].getAverage());
  double accZaverage(buffers[accZ].getAverage());
  double accelerationAngleXZ(atan2(accZaverage, accXaverage));
  double accelerationAngleYZ(atan2(accZaverage, accYaverage));

    //Set state:
  //if(sensorAvailable(accXaverage, accYaverage, accZaverage))
  if(true)
  {
    if (fabs(accelerationAngleXZ) < 0.5)
    {
      theFallDownState->state = FallDownState::lyingOnFront;
    }
    else if(fabs(accelerationAngleXZ) > 2.5)
    {
      theFallDownState->state = FallDownState::lyingOnBack;
    }
    else if (fabs(accelerationAngleYZ) < 0.5)
    {
      theFallDownState->state = FallDownState::lyingOnLeftSide;
    }
    else if(fabs(accelerationAngleYZ) > 2.5)
    {
      theFallDownState->state = FallDownState::lyingOnRightSide;
    }
  }
  //std::cout<<"FallDownDetection: "<<FallDownState::getStateName(theFallDownState->state)<<std::endl;
}


MAKE_KS(Sensor)
