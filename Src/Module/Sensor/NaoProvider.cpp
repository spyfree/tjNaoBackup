#include "NaoProvider.h"
#include "Platform/SystemCall.h"
#include "altypes.h"
#include <iostream>
#include <fstream>

// TODO:Should be not define the save_pic if it is not in debug
#define SAVE_PIC

extern ALPtr<ALBroker>test_pBroker;



//
static const char* jointNames[] =
{
	"HeadYaw",
	"HeadPitch",
	"RShoulderPitch",
	"RShoulderRoll",
	"RElbowYaw",
	"RElbowRoll",
	"LShoulderPitch",
	"LShoulderRoll",
	"LElbowYaw",
	"LElbowRoll",
	"RHipRoll",
	"RHipPitch",
	"RKneePitch",
	"RAnklePitch",
	"RAnkleRoll",
	"LHipYawPitch",
	"LHipRoll",
	"LHipPitch",
	"LKneePitch",
	"LAnklePitch",
	"LAnkleRoll"
};
static const char* sensorNames[] =
{
	"InertialSensor/GyrX",
	"InertialSensor/GyrY",
	"InertialSensor/GyrRef",
	"InertialSensor/AccX",
	"InertialSensor/AccY",
	"InertialSensor/AccZ",
	"InertialSensor/AngleX",
	"InertialSensor/AngleY",
	"Battery/Charge",
	"LFoot/FSR/FrontLeft",
	"LFoot/FSR/FrontRight",
	"LFoot/FSR/RearLeft",
	"LFoot/FSR/RearRight",
	"RFoot/FSR/FrontLeft",
	"RFoot/FSR/FrontRight",
	"RFoot/FSR/RearLeft",
	"RFoot/FSR/RearRight",
	"US",
	"RFoot/Bumper/Right",
	"RFoot/Bumper/Left",
	"LFoot/Bumper/Right",
	"LFoot/Bumper/Left",
	"ChestBoard/Button"
};

static const int numOfJoints = sizeof(jointNames) / sizeof(jointNames[0]);
static const int numOfSensors = sizeof(sensorNames) / sizeof(sensorNames[0]);
////////////////Save pic param
static bool diskfull=false;
// static bool sendImgflag=false;
// static int countkey=0;

NaoProvider::NaoProvider():
				jointangles(numOfJoints,0.0f),
				sensorValues(numOfJoints*2+numOfSensors,0.0f)
{
	registerFastAcess();
	timeS=0.0;
}
NaoProvider::~NaoProvider()
{
}

void NaoProvider::registerFastAcess()
{
	try{
		alfastaccessjoints =
			AL::ALPtr<ALMemoryFastAccess >(new ALMemoryFastAccess());
	} catch(AL::ALError &e){
		std::cout << "Failed to initialize proxy to ALFastAccess for joints"<<std::endl;
	}
	try{
		alfastaccessSensors =
			AL::ALPtr<ALMemoryFastAccess >(new ALMemoryFastAccess());
	} catch(AL::ALError &e){
		std::cout << "Failed to initialize proxy to ALFastAccess for sensors"<<std::endl;
	}
	try{
		initSyncWithALMemory(test_pBroker);
	} catch(AL::ALError &e){
		std::cout << "Failed to initialize sync with al memory"<<std::endl;
	}
}

void NaoProvider::initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker)
{
	TStringArray joints;
	for (int i=0;i<numOfJoints;i++)
	{
		joints.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/Position/Sensor/Value");
	}

	try{
		alfastaccessjoints->ConnectToVariables(broker,joints);
	} catch(AL::ALError& a) {
		std::cout << "NaoEnactor " << a.toString() << std::endl;
	}
	jointangles.reserve(joints.size());
	jointangles.resize(joints.size());

	TStringArray sensors;
	for (int i=0;i<numOfJoints;i++)
	{
		sensors.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/ElectricCurrent/Sensor/Value");
		sensors.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/Temperature/Sensor/Value");
	}
	for(int i = 0; i < numOfSensors; ++i)
	{
		sensors.push_back(std::string("Device/SubDeviceList/") + sensorNames[i] + "/Sensor/Value");
	}
	try{
		alfastaccessSensors->ConnectToVariables(broker,sensors);
	} catch(AL::ALError& a) {
		std::cout << "NaoProvider sensors" << a.toString() << std::endl;
	}
	sensorValues.reserve(sensors.size());
	sensorValues.resize(sensors.size());
}
void NaoProvider::getSensor(SafeJointData*inSafeJointData)
{
	sensoryJointData.angles[0] = double(0.00);
	int idRHipYawPitch=0;
	int i=0;
	alfastaccessjoints->GetValues(jointangles);
	for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
	{
		if(j==JointData::RHipYawPitch)
		{
			idRHipYawPitch=j;
			continue;
		}
		sensoryJointData.angles[j] = double(jointangles[i]);
		i++;
	}
	sensoryJointData.angles[idRHipYawPitch]=sensoryJointData.angles[JointData::LHipYawPitch];
	////////////////sensoryJointData is the original data from the joint,after  the following process it came to the SafeJointData
	for(int i = 0; i < JointData::numOfJoints; ++i)
		if(sensoryJointData.angles[i] != JointData::off)
			inSafeJointData->angles[i] = sensoryJointData.angles[i];
		else if(sensoryJointData.angles[i] == JointData::off)
			inSafeJointData->angles[i] = 0;
	inSafeJointData->timeStamp = sensoryJointData.timeStamp;
}
void NaoProvider::update(SafeJointData*theSafeJointData)
{
	if(true){
		getSensor(theSafeJointData);
	}
}
void NaoProvider::update(SafeSensorData *theSafeSensorData)
{
	try{
		alfastaccessSensors->GetValues(sensorValues);
		int idRHipYawPitch;
		int i = 0;
		for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
		{
			if(j==JointData::RHipYawPitch)
			{
				idRHipYawPitch=j;
				continue;
			}
			sensorData.loads[j] = 1000*sensorValues[i++];
			sensorData.temperatures[j] = sensorValues[i++];
		}
		sensorData.loads[idRHipYawPitch] = sensorData.loads[JointData::LHipYawPitch];
		sensorData.temperatures[idRHipYawPitch] = sensorData.temperatures[JointData::LHipYawPitch];
		//float gyroRef = 0.0f;
		for(int j = 0; j < SensorData::numOfSensors; j++)
		{
			if(j == SensorData::compassX)
				j++;
			else
			{
				sensorData.data[j] = sensorValues[i++];
			}
		}

		//click the number of pressing button

		}catch(AL::ALError &e){
		std::cout << e.toString()<<std::endl;
	}
	///////////sensorData is the original data from the sensor,after  the following process it came to the SafeSensorData
	for(int i = 0; i < SensorData::numOfSensors; ++i)
		theSafeSensorData->data[i] = sensorData.data[i];
	for(int i = 0; i < JointData::numOfJoints; ++i)
	{
		theSafeSensorData->loads[i] = sensorData.loads[i];
		theSafeSensorData->temperatures[i] = sensorData.temperatures[i];
	}
	theSafeSensorData->timeStamp = sensorData.timeStamp;
}
// void NaoProvider::update(FrameInfo *theFrameInfo)
// {
// 	theFrameInfo->time = timeS;
// }
MAKE_KS(NaoProvider)
