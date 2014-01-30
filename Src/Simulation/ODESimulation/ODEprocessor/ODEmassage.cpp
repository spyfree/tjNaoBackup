#include "ODEmassage.h"
#include "Module/Common/VisionHeader.h"
#include "Module/Common/Kinematics.h"
#include <math.h>
#include <iostream>

ODEData::ODEData()
{
	std::cout<<"stat ode data"<<std::endl;
	roboCupCtrl = RoboCupCtrl::getController();
	initOdome = true;
	std::cout<<"ode data"<<std::endl;

}

ODEData::~ODEData()
{

}

void ODEData::GetData()
{
	//get ODEobject reference
	if(roboCupCtrl->getObjectReference("RoboCup.robots")->getNumberOfChildNodes())
	{
		ball = roboCupCtrl->getObjectReference("RoboCup.balls.ball");
		robot = roboCupCtrl->getObjectReference("RoboCup.robots.robot1");
		camera = roboCupCtrl->getObjectReference("RoboCup.robots.robot1.headPan.HeadYaw.headTilt.head.camera");
	}


	ballXData = ball->getPosition().v[0]*1000;//ode里面是m单位，这里应该是mm
	ballYData = ball->getPosition().v[1]*1000;
	robotXData = robot->getPosition().v[0]*1000;
	robotYData = robot->getPosition().v[1]*1000;
	robotRotation = robot->getRotation();

	if (initOdome)
	{
		OdomeInitX = robotXData;
		OdomeInitY = robotYData;
		OdomeInitRot = robotRotation.getZAngle();
		initOdome = false;
	}
	OdomeX = robotXData;
	OdomeY = robotYData;
	OdomeRot = robotRotation.getZAngle();
}
void ODEData::update(SensoryJointData* theSensoryJointData)
{
	selfMessageQueue->SearchMyMessage(idSensoryJointData,idODESimulation, idBehaviorThread,
		(char *)&jointData, sizeof(JointData));

	theSensoryJointData->angles[0] = double(0.00);
	int idRHipYawPitch=0;
	for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
	{
		if(j==JointData::RHipYawPitch)
		{
			idRHipYawPitch=j;
			continue;
		}
		theSensoryJointData->angles[j] = jointData.angles[j];
	}
	theSensoryJointData->angles[idRHipYawPitch]=theSensoryJointData->angles[JointData::LHipYawPitch];
}
void ODEData::update(SafeSensorData *theSafeSensorData)
{
	selfMessageQueue->SearchMyMessage(idSimSensorData,idODESimulation, idBehaviorThread,
		(char*)&sensorData, sizeof(SensorData));

	for(int i = 0; i < SensorData::numOfSensors; ++i)
		//if(sensorData.data[i] != JointData::off || (i == SensorData::us && sensorData.data[i] != 0))
		theSafeSensorData->data[i] = sensorData.data[i];
	//else if(safeSensorData.data[i] == JointData::off)
	//  safeSensorData.data[i] = 0;

	for(int i = 0; i < JointData::numOfJoints; ++i)
	{
		theSafeSensorData->loads[i] = sensorData.loads[i];
		theSafeSensorData->temperatures[i] = sensorData.temperatures[i];
	}

	theSafeSensorData->timeStamp = sensorData.timeStamp;
}

void ODEData::update(OdometryData*theOdometryData)
{
	GetData();
	theOdometryData->rotation = OdomeRot-OdomeInitRot;
	theOdometryData->translation = Vector2<double>(OdomeX -OdomeInitX,OdomeY-OdomeInitY).rotate(OdomeInitRot);
}
void ODEData::update(NaoStructure *theNaoStructure)
{
	theNaoStructure->setLinksJoints(*theSensoryJointData);
	//theNaoStructure->setLinksJoints(*theJointCmd);
	theNaoStructure->uLink[NaoStructure::bodyLink].p = Vector3<double>(0,0,0);
	theNaoStructure->uLink[NaoStructure::bodyLink].R = RotationMatrix();
	Kinematics::execForwardKinematics(*theNaoStructure, NaoStructure::bodyLink);
	Kinematics::recalcBodyPos2SupFoot(*theNaoStructure, Vector3<double>(0,0,0), theNaoStructure->supportingMode);
	Kinematics::execForwardKinematics(*theNaoStructure, NaoStructure::bodyLink);
	theNaoStructure->pCOM = Kinematics::calcCOM(*theNaoStructure);
	//====set bodyangles====
	//(*theNaoStructure).setBodyAngles(Vector3<double>(theSafeSensorData->data[SensorData::angleX], theSafeSensorData->data[SensorData::angleY], 0));
	//In case we get NAN or IND num.... print some thing and halt execution when run on robot!!
	ASSERT(theNaoStructure->pCOM == theNaoStructure->pCOM);

	outNaoStructure();
	
}
MAKE_KS(ODEData)