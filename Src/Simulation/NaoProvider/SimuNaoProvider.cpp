#include "SimuNaoProvider.h"
#include "almemoryproxy.h"
#include "dcmproxy.h"
#include "alvisiondefinitions.h"
#include "almemoryfastaccess.h"
#include "almotionproxy.h"
#include "Platform/SystemCall.h"  //added by qiang
PROCESS_WIDE_STORAGE SimuNaoProvider* SimuNaoProvider::theInstance = 0;
#include <iostream>
#include <fstream>
#include "Platform/GetSetting.h"
#include "Platform/SystemCall.h"
using namespace AL;

#include <alproxy.h>

//// Collor Settings
//static const int DEFAULT_CAMERA_AUTO_GAIN = 0; // AUTO GAIN OFF
//static const int DEFAULT_CAMERA_GAIN = 4;
//
//static const int DEFAULT_CAMERA_AUTO_WHITEBALANCE = 0; // AUTO WB OFF
//static const int DEFAULT_CAMERA_BLUECHROMA = 118; //131;
//static const int DEFAULT_CAMERA_REDCHROMA = 116; //72;
//static const int DEFAULT_CAMERA_BRIGHTNESS = 128;
//static const int DEFAULT_CAMERA_CONTRAST = 64;
//static const int DEFAULT_CAMERA_SATURATION = 128;
//static const int DEFAULT_CAMERA_HUE = 0;
//// Exposure length
//static const int DEFAULT_CAMERA_AUTO_EXPOSITION = 0; // AUTO EXPOSURE ON
//static const int DEFAULT_CAMERA_EXPOSURE = 300;

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


static const char* ledNames[] =
{
	"Face/Led/Red/Left/0Deg",
	"Face/Led/Red/Left/45Deg",
	"Face/Led/Red/Left/90Deg",
	"Face/Led/Red/Left/135Deg",
	"Face/Led/Red/Left/180Deg",
	"Face/Led/Red/Left/225Deg",
	"Face/Led/Red/Left/270Deg",
	"Face/Led/Red/Left/315Deg",
	"Face/Led/Green/Left/0Deg",
	"Face/Led/Green/Left/45Deg",
	"Face/Led/Green/Left/90Deg",
	"Face/Led/Green/Left/135Deg",
	"Face/Led/Green/Left/180Deg",
	"Face/Led/Green/Left/225Deg",
	"Face/Led/Green/Left/270Deg",
	"Face/Led/Green/Left/315Deg",
	"Face/Led/Blue/Left/0Deg",
	"Face/Led/Blue/Left/45Deg",
	"Face/Led/Blue/Left/90Deg",
	"Face/Led/Blue/Left/135Deg",
	"Face/Led/Blue/Left/180Deg",
	"Face/Led/Blue/Left/225Deg",
	"Face/Led/Blue/Left/270Deg",
	"Face/Led/Blue/Left/315Deg",
	"Face/Led/Red/Right/0Deg",
	"Face/Led/Red/Right/45Deg",
	"Face/Led/Red/Right/90Deg",
	"Face/Led/Red/Right/135Deg",
	"Face/Led/Red/Right/180Deg",
	"Face/Led/Red/Right/225Deg",
	"Face/Led/Red/Right/270Deg",
	"Face/Led/Red/Right/315Deg",
	"Face/Led/Green/Right/0Deg",
	"Face/Led/Green/Right/45Deg",
	"Face/Led/Green/Right/90Deg",
	"Face/Led/Green/Right/135Deg",
	"Face/Led/Green/Right/180Deg",
	"Face/Led/Green/Right/225Deg",
	"Face/Led/Green/Right/270Deg",
	"Face/Led/Green/Right/315Deg",
	"Face/Led/Blue/Right/0Deg",
	"Face/Led/Blue/Right/45Deg",
	"Face/Led/Blue/Right/90Deg",
	"Face/Led/Blue/Right/135Deg",
	"Face/Led/Blue/Right/180Deg",
	"Face/Led/Blue/Right/225Deg",
	"Face/Led/Blue/Right/270Deg",
	"Face/Led/Blue/Right/315Deg",
	"Ears/Led/Left/0Deg",
	"Ears/Led/Left/36Deg",
	"Ears/Led/Left/72Deg",
	"Ears/Led/Left/108Deg",
	"Ears/Led/Left/144Deg",
	"Ears/Led/Left/180Deg",
	"Ears/Led/Left/216Deg",
	"Ears/Led/Left/252Deg",
	"Ears/Led/Left/288Deg",
	"Ears/Led/Left/324Deg",
	"Ears/Led/Right/0Deg",
	"Ears/Led/Right/36Deg",
	"Ears/Led/Right/72Deg",
	"Ears/Led/Right/108Deg",
	"Ears/Led/Right/144Deg",
	"Ears/Led/Right/180Deg",
	"Ears/Led/Right/216Deg",
	"Ears/Led/Right/252Deg",
	"Ears/Led/Right/288Deg",
	"Ears/Led/Right/324Deg",
	"ChestBoard/Led/Red",
	"ChestBoard/Led/Green",
	"ChestBoard/Led/Blue",
	"LFoot/Led/Red",
	"LFoot/Led/Green",
	"LFoot/Led/Blue",
	"RFoot/Led/Red",
	"RFoot/Led/Green",
	"RFoot/Led/Blue"
};
static const int numOfJoints = sizeof(jointNames) / sizeof(jointNames[0]);
static const int numOfLeds = sizeof(ledNames) / sizeof(ledNames[0]);
static const int numOfSwitches = 0;
static const int numOfSensors = sizeof(sensorNames) / sizeof(sensorNames[0]);
static const float JointHardness = 0.80f;
static const float M1R1 = 0.1012;
static const float M1R2 = 0.0658;
static const float M2R1 = 0.1227;
static const float M2R2 = 0.1066;
const float jointMax[JointData::numOfJoints-2]={
	//head
	M2R2, M2R1,
	//right arm
	M2R2, M2R1, M2R1, M2R2,
    //left arm
    M2R2, M2R1, M2R1, M2R2,
	//right leg  , notice: no RHipYawPitch
	      M1R2, M1R2,
	M1R1, M1R2, M1R1,
	//left leg
	M1R1,M1R2, M1R2,
	M1R1, M1R2, M1R1

};
SimuNaoProvider::SimuNaoProvider():
		theImage(NULL),
		jointangles(numOfJoints,0.0f),
		timeOffset(0),
		jointValues(numOfJoints,0.0f),
		actuatorProxy(NULL),
		camera(NULL),
		sensorValues(numOfJoints*2+numOfSensors,0.0f)

{
SimuNaoProvider::theInstance = this;
	isConnect=false;
	dcmProxy=NULL;
	myMotionProxy = NULL;
	myMemoryProxy=NULL;
	theImage=new TJImage;
	//selfSimulationData=SimulationData::getInstance();
    //alfastaccessJoints = NULL;
	//alfastaccessSensors = NULL;
	for (int i=0;i<numOfJoints;i++)
	{
		currentjointvalue[i] = 0.0f;
	}
   cameraconfig = new CameraConfig();
   std::cout<<"end of simu nao "<<std::endl;

}
SimuNaoProvider::~SimuNaoProvider()
{

	camera->callVoid( "unsubscribe", GVM_name );
	if(theImage!=NULL)
	{
		theImage=NULL;
		delete theImage;
	}
	//use to contol camera
	if(camera!=NULL)
	{
		camera=NULL;
	delete camera;
	}
	if (dcmProxy!=NULL)
	{
		dcmProxy = NULL;
		delete dcmProxy;
	}
	if (myMemoryProxy!=NULL)
	{
		delete myMemoryProxy;
		myMemoryProxy = NULL;
	}
	if (myMotionProxy!=NULL)
	{
		delete myMotionProxy;
		myMotionProxy=NULL;
	}
//	if (alfastaccessJoints!=NULL)  //added by qiang
//	{
//		alfastaccessJoints = NULL;
//		delete alfastaccessJoints;
//	}
//	if (alfastaccessSensors!=NULL)
//	{
//		alfastaccessSensors = NULL;
//		delete alfastaccessSensors;
//	}
}

void SimuNaoProvider::registerCam()
{
	GVM_name = "tutorial_GVM";
	GVM_name = camera->call<std::string>( "subscribe", GVM_name, cameraconfig->resolution,
		cameraconfig->colorspace, cameraconfig->fps );
    cameraActive=true;
}

void SimuNaoProvider::initDcm()
{
	ALValue delta = 0;
	timeOffset = (int)dcmProxy->getTime(delta) - SystemCall::getCurrentSystemTime(); 
	initDCMAliases();
	initDCMCommands();


	//setBodyHardness(0.8f);

	//sendJoints();

}
void SimuNaoProvider::initDCMAliases()
{
	ALValue positionCommandsAlias;
	positionCommandsAlias.arraySetSize(2);
	positionCommandsAlias[0] = std::string("AllActuatorPosition");
	
	ALValue hardCommandsAlias;
	hardCommandsAlias.arraySetSize(2);
	hardCommandsAlias[0] = std::string("AllActuatorHardness");

	positionCommandsAlias[1].arraySetSize(numOfJoints);
	hardCommandsAlias[1].arraySetSize(numOfJoints);
	for (int i=0;i<numOfJoints;i++)
	{
		positionCommandsAlias[1][i] = std::string(jointNames[i]) + "/Position/Actuator/Value";
		hardCommandsAlias[1][i] = std::string(jointNames[i]) + "/Hardness/Actuator/Value";
	}
	
	dcmProxy->createAlias(positionCommandsAlias);
	dcmProxy->createAlias(hardCommandsAlias);

}

void SimuNaoProvider::initDCMCommands()
{
	joint_command.arraySetSize(6);
	joint_command[0] = std::string("AllActuatorPosition");
	//joint_command[1] = string("Merge");
	joint_command[1] = std::string("Merge");
	joint_command[2] = std::string("time-separate");
	joint_command[3] = 0; //importance level
	joint_command[4].arraySetSize(1); //list of time to send commands
	
	hardness_command.arraySetSize(6);
	hardness_command[0] = std::string("AllActuatorHardness");
	//hardness_command[1] = string("Merge");
    hardness_command[1] = std::string("Merge");
	hardness_command[2] = std::string("time-separate");
	hardness_command[3] = 0; //importance level
	hardness_command[4].arraySetSize(1); //list of time to send commands
	
    joint_command[5].arraySetSize(numOfJoints);
    hardness_command[5].arraySetSize(numOfJoints);
	
}


bool SimuNaoProvider::connectNao(std::string ipStr)
{
	bool isOK = true;
	if(isConnect==false)
	{
		try
		{
			std::string brokerName = "TJArkNaoPC";
			std::string brokerIP = "";
			int brokerPort = 0;
			// Default parent broker IP
			std::string parentBrokerIP = ipStr;
			// Default parent broker port
			int parentBrokerPort = kBrokerPort;
			// init the broker with its ip and port, and the ip and port of a parent broker, if exist
			/* ALPtr<ALBroker> broker=(brokerName, brokerIP, brokerPort, parentBrokerIP, parentBrokerPort);*/
			 AL::ALBroker::Ptr broker = AL::ALBroker::createBroker(brokerName,brokerIP, brokerPort,parentBrokerIP, parentBrokerPort);
			isConnect=true;
			if(camera==NULL)
			camera= new AL::ALProxy(broker,"ALVideoDevice");
			registerCam();
			setcamera();
			/*
			if (alfastaccessJoints==NULL)
			{
				alfastaccessJoints = new ALMemoryFastAccess;
				alfastaccessSensors = new ALMemoryFastAccess;
				initSyncWithALMemory(broker);
			}
			*/
			if(myMotionProxy==NULL)
			{

			try{
				myMotionProxy = new AL::ALMotionProxy(broker);
			} catch(AL::ALError &e){
				std::cout << "Failed to initialize myMotionProxy"<<std::endl;
			}
			}

			if(myMemoryProxy==NULL)
			{
				try
				{
					myMemoryProxy = new AL::ALMemoryProxy(broker);
				} catch(AL::ALError &e){
					std::cout << "Failed to initialize myMemoryProxy"<<std::endl;
				}
			}

			if (dcmProxy == NULL)
			{
				dcmProxy = new AL::DCMProxy(broker);
				initDcm();
			}
			isOK =  true;
		}
		catch (ALError &e) {
			//log->error("NaoMain", "Could not call the fetchNextLocal method of the "
			//"ALVideoDevice module");
			std::cout<<"can't get a picture,please check the net condition"<<std::endl;
			isOK = false;
		}
		//=====connect tjpro mainmodule
		/*
		if(isOK){
			try
			{
				actuatorProxy = new AL::ALProxy("MainModule", ipStr,9559);
				std::cout<<"Connected to Remote TJRobot MainModule"<<std::endl;
			}
			catch (ALError &e)
			{
				std::cout<<"can't get a Actuator Proxy"<<std::endl;
			}
		}
		*/
		
	}
return true;
}

void SimuNaoProvider::changeCam()
{
   //cameraconfig->CamChanged=true;
   if (cameraconfig->usingTopCam==true)
   {
	   cameraconfig->setTopCamfalse();
   }
   else
   {
	   cameraconfig->setTopCamtrue();
   }
}

void SimuNaoProvider::getPic(TJImage &InputImage)
{

if(isConnect==true)
{
	// You only have to call the function "register" with those parameters and the 
	// VIM will be responsible for driver initialisation and buffer's management.
	//registerCam();
	//setcamera();
	if(cameraconfig->CamChanged==true)
	{
		if (cameraconfig->usingTopCam==true)
		{
			camera->callVoid( "setParam", kCameraSelectID,1);
			cameraconfig->setCamChangedFalse();
		}
		else
		{
			camera->callVoid( "setParam", kCameraSelectID,0);
			cameraconfig->setCamChangedFalse();
		}

	}
	image.arraySetSize(7);
	image =(( camera->call<AL::ALValue>("getImageRemote",GVM_name)));
	int width = (int) (image)[0];
	int height = (int) (image)[1];
	int nbLayers = (int) (image)[2];
	int colorSpace = (int) (image)[3];
	long long timeStamp = ((long long)(int)image[4])*1000000LL + (long long)(int)image[5];
	// image[4] is the number of seconds, image[5] the number of microseconds

	// You can get the pointer to the image data and its size
	const unsigned char*dataPointer=static_cast<const unsigned char*>(image[6].GetBinary());
	int size = (int)(image)[6].getSize();
	memcpy(InputImage.unCorrectedRaw, dataPointer,size);
////////////可能要加图片其他数据的传递

	//sendPoint.write(&size,sizeof(int));
	//camera->callVoid( "unRegister", GVM_name );
}

}

/*void SimuNaoProvider::readConfig()
{
	std::string buffer;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/cameraSetting.cfg";
	std::cout<<path<<std::endl;
	std::ifstream stream(path.c_str());
	if (!stream.is_open())
	{
		std::cout<<path<<" Open camera configure file failed "<<std::endl;
	}
	const int cfgLine = 30;
	int temp=0;
	int config[cfgLine/2];
	int j=0;
	for(int i=0;i<cfgLine;i++)
	{
		// ignore space
		while(isspace(stream.peek()))
			stream.get();
		if (stream.peek()== '#')
		{
			while(stream.peek()!='\n')


				std::cout<<"vision debug"<<stream.get()<<std::endl;

            stream.get();
				//std::cout<<"vision debug"<<stream.get()<<std::endl;

			continue;
		}
		else
		{
			stream>>temp;
			config[j]=temp;
			j++;
			//std::cout<<"temp----"<<temp<<std::endl;
		}

	}//end for(int i
	resolution=config[0];
	colorspace=config[1];
	fps=config[2];
	cameraID=config[3];

	DEFAULT_CAMERA_AUTO_GAIN=config[4];
	DEFAULT_CAMERA_GAIN=config[5];
	DEFAULT_CAMERA_AUTO_WHITEBALANCE=config[6];
	DEFAULT_CAMERA_BLUECHROMA=config[7];
	DEFAULT_CAMERA_REDCHROMA=config[8];
	DEFAULT_CAMERA_BRIGHTNESS=config[9];
	DEFAULT_CAMERA_CONTRAST=config[10];
	DEFAULT_CAMERA_SATURATION=config[11];
	DEFAULT_CAMERA_HUE=config[12];

	DEFAULT_CAMERA_AUTO_EXPOSITION=config[13];
	DEFAULT_CAMERA_EXPOSURE=config[14];
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_BLUECHROMA is "<<DEFAULT_CAMERA_BLUECHROMA<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_REDCHROMA is "<<DEFAULT_CAMERA_REDCHROMA<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_BRIGHTNESS is "<<DEFAULT_CAMERA_BRIGHTNESS<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_CONTRAST is "<<DEFAULT_CAMERA_CONTRAST<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_SATURATION is "<<DEFAULT_CAMERA_SATURATION<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_HUE is "<<DEFAULT_CAMERA_HUE<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_AUTO_EXPOSITION is "<<DEFAULT_CAMERA_AUTO_EXPOSITION<<std::endl;
	std::cout<<"in ImageProvider, DEFAULT_CAMERA_EXPOSURE is "<<DEFAULT_CAMERA_EXPOSURE<<std::endl;



}*/


void SimuNaoProvider::setcamera()
{
	//change the camera
	camera->callVoid( "setParam", kCameraSelectID,cameraconfig->cameraID);
	/*******************  we must write below in a configure file   *****************************/
	// Turn off auto settings
	// Auto exposure
	try {
		camera->callVoid("setParam", kCameraAutoExpositionID,
			cameraconfig->DEFAULT_CAMERA_AUTO_EXPOSITION);
	} catch (ALError &e)
	{
		std::cout<< "Couldn't set AutoExposition"<<std::endl;
	}
	int autoExposition=camera->call<int>("getParam",kCameraAutoExpositionID);
	std::cout<<"autoExposition is "<<autoExposition<<std::endl;
	// Auto white balance
	try {
		camera->callVoid("setParam", kCameraAutoWhiteBalanceID,
			cameraconfig->DEFAULT_CAMERA_AUTO_WHITEBALANCE);

	} catch (ALError &e){
		std::cout<<"Couldn't set AutoWhiteBalance"<<std::endl;
	}

	// Set camera defaults
	// brightness
	try {
		camera->callVoid("setParam", kCameraBrightnessID,
			cameraconfig->DEFAULT_CAMERA_BRIGHTNESS);
	} catch (ALError &e){
		std::cout<<"Couldn't set Brightness "<<std::endl;
	}
	// contrast
	try {
		camera->callVoid("setParam", kCameraContrastID,
			cameraconfig->DEFAULT_CAMERA_CONTRAST);
	} catch (ALError &e){
		std::cout<< "Couldn't set Contrast"<<std::endl;
	}
	// white balance
	try{
		camera->callVoid("setParam",kCameraWhiteBalanceID,
			cameraconfig->DEFAULT_CAMERA_WHITEBALANCE);
	} catch(ALError &e){
		std::cout<<"Couldn't set WhiteBalance"<<std::endl;
	}
	
	// Exposure length
	try {
		camera->callVoid("setParam",kCameraExposureID,
			cameraconfig->DEFAULT_CAMERA_EXPOSURE);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Exposure"<<std::endl;
	}
	// Gain
	try {
		camera->callVoid("setParam",kCameraGainID,
			cameraconfig->DEFAULT_CAMERA_GAIN);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Gain"<<std::endl;
	}
	// Saturation
	try {
		camera->callVoid("setParam",kCameraSaturationID,
			cameraconfig->DEFAULT_CAMERA_SATURATION);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Saturation"<<std::endl;
	}
	// Hue
	try {
		camera->callVoid("setParam",kCameraHueID,
			cameraconfig->DEFAULT_CAMERA_HUE);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Hue"<<std::endl;
	}
	try {
		camera->callVoid("setParam",kCameraExposureID,
			cameraconfig->DEFAULT_CAMERA_EXPOSURE);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Exposure"<<std::endl;
	}
}

void SimuNaoProvider::outSafeSensorData(SafeSensorData& inpuSafeSensorData)
{
	SensorData tempsensordata;
	std::vector<float> tempSensorVector;
	try{
		//alfastaccessSensors->GetValues(sensorValues);		

// 		int idRHipYawPitch;
// 		int i = 0;
// 		for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
// 		{
// 			if(j==JointData::RHipYawPitch)
// 			{
// 				idRHipYawPitch=j;
// 				continue;
// 			}
// 			tempsensordata.loads[j] = 1000*sensorValues[i++];
// 			tempsensordata.temperatures[j] = sensorValues[i++];
// 		}
// 		tempsensordata.loads[idRHipYawPitch] = tempsensordata.loads[JointData::LHipYawPitch];
// 		tempsensordata.temperatures[idRHipYawPitch] = tempsensordata.temperatures[JointData::LHipYawPitch];

		//float gyroRef = 0.0f;

		for (int i=0;i<SensorData::numOfSensors;i++)
		{
			const char * tempName = SensorData::getNaoSensorName(SensorData::Sensor(i));
			if( ~strcmp(tempName,"unknown") )
			{
				continue;
			}
			else
			{
				float tempValue = myMemoryProxy->getData(tempName);
				tempSensorVector.push_back(tempValue);
			}
		}
		

// 		for(int j = 0; j < SensorData::numOfSensors; j++)
// 		{
// 			if(j == SensorData::compassX)
// 				j++;
// 			else
// 			{
// 				tempsensordata.data[j] = sensorValues[i++];
// 			}
// 		}


	}catch(AL::ALError &e){
		std::cout << e.toString()<<std::endl;
	}
	///////////tempsensordata is the original data from the sensor,after  the following process it came to the inpuSafeSensorData
	int tempIndex = 0;
	for(int i = 0; i < SensorData::numOfSensors; ++i)
	{
		const char * tempName = SensorData::getNaoSensorName(SensorData::Sensor(i));
		if (~strcmp(tempName,"unknown"))
		{
			inpuSafeSensorData.data[i] = 0;
		}
		else
		{
			inpuSafeSensorData.data[i] = tempSensorVector[tempIndex];
			tempIndex++;
		}		
	}

	for(int i = 0; i < JointData::numOfJoints; ++i)
	{
		inpuSafeSensorData.loads[i] = 0;  //tempsensordata.loads[i];
		inpuSafeSensorData.temperatures[i] =0;  //tempsensordata.temperatures[i];
	}
	inpuSafeSensorData.timeStamp = tempsensordata.timeStamp;


}
void SimuNaoProvider::outSafeJointData(SafeJointData& inputSafeJointData)
{
   // upSensorData();
	SafeJointData tempSafeJointData;
	tempSafeJointData.angles[0] = double(0.00);
	jointangles = myMotionProxy->getAngles("Body",TRUE);
	for (int i=JointData::HeadYaw;i<=JointData::HeadPitch;i++)
	{
		tempSafeJointData.angles[i]=double(jointangles[i-1]);
	}
	for (int i=JointData::RShoulderPitch;i<=JointData::RElbowRoll;i++)
	{
		tempSafeJointData.angles[i]=double(jointangles[i+15]);
	}
	for (int i=JointData::LShoulderPitch;i<=JointData::LElbowRoll;i++)
	{
		tempSafeJointData.angles[i]=double(jointangles[i-5]);
	}
	for (int i=JointData::RHipYawPitch;i<=JointData::RAnkleRoll;i++)
	{
		tempSafeJointData.angles[i]=double(jointangles[i+1]);
	}
	for (int i=JointData::LHipYawPitch;i<=JointData::LAnkleRoll;i++)
	{
		tempSafeJointData.angles[i]=double(jointangles[i-11]);
	}
//	int idRHipYawPitch=0;
//	int i=0;
//	alfastaccessJoints->GetValues(jointangles);
//	for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
//	{
//		if(j==JointData::RHipYawPitch)
//		{
//			idRHipYawPitch=j;
//			continue;
//		}
//		tempSafeJointData.angles[j] = double(jointangles[i]);
//		i++;
//	}
//	tempSafeJointData.angles[idRHipYawPitch]=tempSafeJointData.angles[JointData::LHipYawPitch];

	////////////////tempSafeJointData is the original data from the joint,after  the following process it came to the inputSafeJointData

	for(int i = 0; i < JointData::numOfJoints; ++i)
		if(tempSafeJointData.angles[i] != JointData::off)
			inputSafeJointData.angles[i] = tempSafeJointData.angles[i];
		else if(tempSafeJointData.angles[i] == JointData::off)
			inputSafeJointData.angles[i] = 0;
	inputSafeJointData.timeStamp = tempSafeJointData.timeStamp;

}
void SimuNaoProvider::closeConnect()
{
	isConnect=false;
	camera=NULL;
	//alfastaccessJoints = NULL;  //added by qiang
	//alfastaccessSensors = NULL;
	setBodyHardness(0.0f);
}

void SimuNaoProvider::sendSimJoints(const JointCmd& outMotionJointData,int portID,unsigned time_ms /* = 2000 */)
{
	if(isConnect)
	{
		int j=portID - 1;//j从0开始计数的
		alfastaccessJoints->GetValues(jointValues);

		if(outMotionJointData.angles[portID]!=JointData::off&&outMotionJointData.angles[portID]!=JointData::ignore)
		{
			joint_command[5][j].arraySetSize(1);
			joint_command[5][j][0] = (float)outMotionJointData.angles[portID];
			hardness_command[5][j].arraySetSize(1);
			hardness_command[5][j][0] = JointHardness;

		}
		else 
		{
			joint_command[5][j].arraySetSize(1);
			joint_command[5][j][0] = jointValues[j];
			hardness_command[5][j].arraySetSize(1);
			hardness_command[5][j][0] = 0.0f;
		}

		hardness_command[4][0] = dcmProxy->getTime(0);
		joint_command[4][0] = dcmProxy->getTime(time_ms);

		try
		{
			dcmProxy->setAlias(hardness_command);
			dcmProxy->setAlias(joint_command);
		}
		catch (AL::ALError& a)
		{
			std::cout<<"dcm value and hardness set error  "<<std::endl;
		}
	
		
	}

}

void SimuNaoProvider::sendJoints(const JointCmd& outMotionJointData,unsigned time_ms)
{
	if(isConnect)
	{
		int j=0;
		// TODO::The alfastaccessJoints has been deleted,the call for sendJoints function may cause mistake,should be changed here by motion module
		alfastaccessJoints->GetValues(jointValues);
		for (int i=JointData::HeadYaw;i<JointData::numOfJoints;i++)
		{

			if(i == JointData::RHipYawPitch) // missing on Nao
				++i;
			if(outMotionJointData.angles[i]!=JointData::off&&outMotionJointData.angles[i]!=JointData::ignore)
			{
				joint_command[5][j].arraySetSize(1);
				//if (i==JointData::RShoulderRoll||i==JointData::RElbowYaw||i==JointData::RElbowRoll)
				//{
				//	//joint_command[5][j][0] = SafetyCheck(jointValues[j], (float)-outMotionJointData.angles[i], j);//(float)outMotionJointData->angles[i];
				//	joint_command[5][j][0] = (float)-outMotionJointData.angles[i];
				//} 
				//else
				//{
					//joint_command[5][j][0] = SafetyCheck(jointValues[j], (float)outMotionJointData.angles[i], j);//(float)outMotionJointData->angles[i];
					joint_command[5][j][0] = (float)outMotionJointData.angles[i];
				//}
				hardness_command[5][j].arraySetSize(1);
				hardness_command[5][j][0] = JointHardness;

			}
			else 
			{
				joint_command[5][j].arraySetSize(1);
				joint_command[5][j][0] = jointValues[j];
				hardness_command[5][j].arraySetSize(1);
				hardness_command[5][j][0] = 0.0f;
			}
			j++;


					
		hardness_command[4][0] = dcmProxy->getTime(0);
		//int dcmDelay = 2000;
		//joint_command[4][0] = (int)SystemCall::getCurrentSystemTime() + timeOffset + dcmDelay;
		joint_command[4][0] = dcmProxy->getTime(time_ms);

	  try
		{
			dcmProxy->setAlias(hardness_command);
			dcmProxy->setAlias(joint_command);
		}
		catch (AL::ALError& a)
		{
			std::cout<<"dcm value and hardness set error  "<<std::endl;
		}
	}

  }

}
float SimuNaoProvider::SafetyCheck(float currentVal,float toCheck,int i)
{
	float absDiffInRad = fabs(currentVal - toCheck);
	float allowedDiffInRad = jointMax[i];
	if (absDiffInRad > allowedDiffInRad)
	{
		if (toCheck > currentVal)
		{
			return (currentVal + allowedDiffInRad);
		} 
		else
		{
			return (currentVal - allowedDiffInRad);
		}

	} 
	
    return toCheck;
}


void SimuNaoProvider::initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker)
{
	TStringArray sensors;
	for (int i=0;i<numOfJoints;i++)
	{
		sensors.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/Position/Sensor/Value");
		//sensors.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/ElectricCurrent/Sensor/Value");
		//sensors.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/Temperature/Sensor/Value");
	}
	//alfastaccessJoints->ConnectToVariables(broker,sensors);
	jointangles.reserve(sensors.size());
	jointangles.resize(sensors.size());
	jointValues.reserve(sensors.size());
	jointValues.resize(sensors.size());

   TStringArray  sensordata;
   for (int i=0;i<numOfJoints;i++)
   {
	   sensordata.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/ElectricCurrent/Sensor/Value");
	   sensordata.push_back(std::string("Device/SubDeviceList/") + jointNames[i] + "/Temperature/Sensor/Value");
   }
	for (int i=0;i<numOfSensors;i++)
	{
       sensordata.push_back(std::string("Device/SubDeviceList/") + sensorNames[i] + "/Sensor/Value");
	}
	//alfastaccessSensors->ConnectToVariables(broker,sensordata);
	sensorValues.reserve(sensordata.size());
    sensorValues.resize(sensordata.size());
}

void SimuNaoProvider::upSensorData()
{
	alfastaccessSensors->GetValues(sensorValues);
	int idRHipYawPitch=0;
	int i=0;
	for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
	{
		if(j==JointData::RHipYawPitch)
		{
			idRHipYawPitch=j;
			continue;
		}
		loads[j] = float(sensorValues[i++]);
        temperatures[j] = float(sensorValues[i++]);
		
	}
	loads[idRHipYawPitch] = loads[JointData::LHipYawPitch];
    temperatures[idRHipYawPitch] = temperatures[JointData::LHipYawPitch];

	for (int j=JointData::HeadYaw;j<JointData::numOfJoints;j++)
	{
         std::cout<<j<<"  "<<loads[j]<<std::endl;
	}
   for (int j=0;j<numOfSensors;j++)
   {
	   sensordata[j] = float(sensorValues[i++]);
	   //cout<<sensordata[j]<<endl;
   }

}


void SimuNaoProvider::setBodyHardness(float hardness)  // use it when close the dialog
{

	for (int i=0;i<numOfJoints;i++)
	{
		hardness_command[5][i].arraySetSize(1);
		hardness_command[5][i][0] = hardness;

	}

	hardness_command[4][0] = dcmProxy->getTime(0);

	try
	{
		dcmProxy->setAlias(hardness_command);
	}
	catch (AL::ALError* a)
	{
		std::cout<<"DCM Hardness set error"<<std::endl;
	}

}




