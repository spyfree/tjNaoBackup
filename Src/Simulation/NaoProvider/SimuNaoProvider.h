#ifndef SimuNaoProvider_H
#define SimuNaoProvider_H
#include <string>
#include "Module/Representations/BaseProvider.h"
#include "Module/Vision/TJImage.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "alvalue.h"
#include "altypes.h"
#include "albroker.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "alptr.h"  //added by cai
#include "Simulation/SimulationData.h"

//DEFINE_KS(SimuNaoProvider)
//PROVIDES(TJImage)
//END_KS
//#define onlySendArmData



namespace AL 
{
	class ALMemoryProxy;
	class ALMotionProxy;
	class ALProxy;
	class ALValue;
	class ALBroker;
	class DCMProxy;
	class ALMemoryFastAccess;  //added by qiang
};

class SimuNaoProvider/*:public SimuNaoProviderBase,BaseProvider*/
{
public:
	static SimuNaoProvider* getInstance()
	{
		if(theInstance==NULL)
		{
			theInstance=new SimuNaoProvider();
		}
		return theInstance;
	}
	~SimuNaoProvider();
    
	void sendSimJoints(const JointCmd& outMotionJointData,int portID,unsigned time_ms = 2000);
	void sendJoints(const JointCmd& outMotionJointData,unsigned time_ms = 2000);   //added for sending jointdata and hardness by DCM
    float SafetyCheck(float jointvalue,float motionvalue,int i);

//	void sendJoints(unsigned time_ms = 2000);   //added for sending jointdata and hardness by DCM
	bool isConnected(){return isConnect;}

    void getcurrentvalue(float *currentjointvalue);
	float currentjointvalue[JointData::numOfJoints-2];

public:
	
	void initDcm();

	void outImage();
	//use to control dcm (JointData ,Leds)

	bool connectNao(std::string ipStr);
	/*ALPtr<ALBroker> broker;*/
	//void readConfig();
	void registerCam();
	void setcamera();
	void getPic(TJImage &InputImage);
	void changeCam();
	//use to contol camera
	TJImage *theImage;
	AL::ALProxy *camera;
	AL::ALProxy *actuatorProxy;
	AL::ALValue image;

	void closeConnect();
    SimulationData *selfSimulationData;
	void outSafeJointData(SafeJointData& inputSafeJointData);
	void outSafeSensorData(SafeSensorData& inpuSafeSensorData);

	AL::ALPtr<AL::ALMemoryFastAccess> alfastaccessJoints;  
	std::vector<float> jointangles;   //get sensordata in function of outsensordata
	std::vector<float> jointValues;   //get jointdata to safecheck and send
	void initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker);
	std::vector<float> sensorValues;
    void upSensorData();
	float loads[JointData::numOfJoints]; 
	float temperatures[JointData::numOfJoints]; 
	float sensordata[21];
	AL::ALPtr<AL::ALMemoryFastAccess> alfastaccessSensors; 

 //  SensoryJointData theSensoryJointData;
	//added for sending jointdata and hardness by DCM
	AL::ALValue joint_command;
	AL::ALValue hardness_command;
	void initDCMAliases();
	void initDCMCommands();

	void setBodyHardness(float hardness);
	AL::DCMProxy* dcmProxy;
	AL::ALMotionProxy * myMotionProxy;
	AL::ALMemoryProxy * myMemoryProxy;
    CameraConfig* cameraconfig;
private:
	PROCESS_WIDE_STORAGE_STATIC SimuNaoProvider* theInstance; /**< The only instance of this module. */
	SimuNaoProvider();
	bool isConnect;
	bool cameraActive;
	int timeOffset;
	std::string GVM_name;
	//int width;

	//int resolution;
	//int ColorSpace;
	//int fps ;
	//int CameraID;
	//// Color Settings
	//int DEFAULT_CAMERA_AUTO_GAIN;// = 0; // AUTO GAIN OFF
	//int DEFAULT_CAMERA_GAIN;// = 4;
	//int DEFAULT_CAMERA_AUTO_WHITEBALANCE;// = 0; // AUTO WB OFF
	//int DEFAULT_CAMERA_BLUECHROMA;// = 118;//158; //131;
	//int DEFAULT_CAMERA_REDCHROMA;// = 116;//68; //72;
	//int DEFAULT_CAMERA_BRIGHTNESS;// = 128;
	//int DEFAULT_CAMERA_CONTRAST;// = 64;
	//int DEFAULT_CAMERA_SATURATION;// = 128;
	//int DEFAULT_CAMERA_HUE;// = 0;
	//// Exposure length
	//int DEFAULT_CAMERA_AUTO_EXPOSITION;// = 0; // AUTO EXPOSURE ON
	//int DEFAULT_CAMERA_EXPOSURE;// = 300;
	

};
#endif
