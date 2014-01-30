#include "ImageProvider.h"
#include "Platform/SystemCall.h"
#include "altypes.h"
#include "alimage.h"
#include <iostream>
#include <fstream>

// TODO:Should be not define the save_pic if it is not in debug
#define SAVE_PIC
//#include "Platform/GetSetting.h"
// Collor Settings
//static const int DEFAULT_CAMERA_AUTO_GAIN = 0; // AUTO GAIN OFF
//static const int DEFAULT_CAMERA_GAIN = 4;
//static const int DEFAULT_CAMERA_AUTO_WHITEBALANCE = 0; // AUTO WB OFF
//static const int DEFAULT_CAMERA_BLUECHROMA = 118;//158; //131;
//static const int DEFAULT_CAMERA_REDCHROMA = 116;//68; //72;
//static const int DEFAULT_CAMERA_BRIGHTNESS = 128;
//static const int DEFAULT_CAMERA_CONTRAST = 64;
//static const int DEFAULT_CAMERA_SATURATION = 128;
//static const int DEFAULT_CAMERA_HUE = 0;
// Exposure length
//static const int DEFAULT_CAMERA_AUTO_EXPOSITION = 0; // AUTO EXPOSURE ON
//static const int DEFAULT_CAMERA_EXPOSURE = 300;

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

ImageProvider::ImageProvider():
				jointangles(numOfJoints,0.0f),
				sensorValues(numOfJoints*2+numOfSensors,0.0f),
				chestButton(new ClickButton()),
				leftFootButton(new ClickButton()),
				rightFootButton(new ClickButton())
{
	initCamera();
	registerCamera();
	setCamera();
	registerFastAcess();
	timeS=0.0;
}
ImageProvider::~ImageProvider()
{
	//if (classifiedPic!=NULL)
	//{
	//	delete classifiedPic;
	//	classifiedPic=NULL;
	//}
	unregisterCamera();
}

void ImageProvider::initCamera()
{
	//readConfig();

	// First you have to choose a name for your GVM
	GVM_name = "tutorial_GVM";
	// Then specify the resolution among : kVGA ( 640 * 480 ), kQVGA ( 320 * 240 ),kQQVGA ( 160 * 120 ).
	// definitions in extern/c/aldebaran/alcommon/include/alvisiondefinitions.h and alvisionimage.h )
	//resolution=kQVGA;
	// Then specify the color space desired among : kYuvColorSpace, kYUVColorSpace,
	 //kYUV422InterlacedColorSpace, kRGBColorSpace.
	//colorspace=kYUVColorSpace;
	//select the minimal number of frames per second ( fps ) among : 5, 10, 15, and 30 fps.
	//fps = 5;//15;
	//CameraID 0 is the top camera,1 is the bottom camera
	//cameraID=0;
}
/*
void ImageProvider::readConfig()
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

void ImageProvider::registerCamera()
{
	try
	{
		camera=test_pBroker->getProxy("ALVideoDevice");
		cameraActive=true;
	}
	catch(AL::ALError &e)
	{
		std::cout<<" fail to get a proxy to ALVideoDevice "<<std::endl;
		cameraActive=false;
		return;
	}

	try
	{
		GVM_name = camera->call<std::string>( "subscribe", GVM_name, theCameraConfig->resolution,
		theCameraConfig->colorspace, theCameraConfig->fps );
	}
	catch(ALError &e)
	{
		std::cout<<" fail to register camera "<<std::endl;
		cameraActive=false;
	}
	///////////////////////////////////////////////////
// 	try
// 	{
// 		dcm=test_pBroker->getProxy("DCM");
// 	}
// 	catch (ALError* e)
// 	{
// 		std::cout<<" fail to register DCM "<<std::endl;
//
// 	}
}

void ImageProvider::setCamera()
{
	//change the camera
  camera->callVoid( "setParam", kCameraSelectID,theCameraConfig->cameraID);

/*******************  we must write below in a configure file   *****************************/
	// Turn off auto settings
	// Auto exposure
	try {
		camera->callVoid("setParam", kCameraAutoExpositionID,
			theCameraConfig->DEFAULT_CAMERA_AUTO_EXPOSITION);
	} catch (ALError &e)
	{
		std::cout<< "Couldn't set AutoExposition"<<std::endl;
	}
	// Auto white balance
	try {
		camera->callVoid("setParam", kCameraAutoWhiteBalanceID,
			theCameraConfig->DEFAULT_CAMERA_AUTO_WHITEBALANCE);

	} catch (ALError &e){
		std::cout<<"Couldn't set AutoWhiteBalance"<<std::endl;
	}

	// Set camera defaults
	// brightness
	try {
		camera->callVoid("setParam", kCameraBrightnessID,
			theCameraConfig->DEFAULT_CAMERA_BRIGHTNESS);
	} catch (ALError &e){
		std::cout<<"Couldn't set Brightness "<<std::endl;
	}
	// contrast
	try {
		camera->callVoid("setParam", kCameraContrastID,
			theCameraConfig->DEFAULT_CAMERA_CONTRAST);
	} catch (ALError &e){
		std::cout<< "Couldn't set Contrast"<<std::endl;
	}
	// White Balance
	try{
		camera->callVoid("setParam",kCameraWhiteBalanceID,
			theCameraConfig->DEFAULT_CAMERA_WHITEBALANCE);
	} catch(ALError &e){
		std::cout<<"Couldn't set WhiteBalance"<<std::endl;
	}

	
	// Exposure length
	try {
		camera->callVoid("setParam",kCameraExposureID,
			theCameraConfig->DEFAULT_CAMERA_EXPOSURE);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Exposure"<<std::endl;
	}
	// Gain
	try {
		camera->callVoid("setParam",kCameraGainID,
			theCameraConfig->DEFAULT_CAMERA_GAIN);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Gain"<<std::endl;
	}

	// Saturation
	try {
		camera->callVoid("setParam",kCameraSaturationID,
			theCameraConfig->DEFAULT_CAMERA_SATURATION);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Saturation"<<std::endl;
	}

	// Hue
	try {
		camera->callVoid("setParam",kCameraHueID,
			theCameraConfig->DEFAULT_CAMERA_HUE);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Hue"<<std::endl;
	}
    //to do :a bug
	try {
		camera->callVoid("setParam",kCameraExposureID,
			theCameraConfig->DEFAULT_CAMERA_EXPOSURE+1);
	} catch (ALError &e) {
		std::cout<<"Couldn't set Exposure"<<std::endl;
	}

}

void ImageProvider::releaseImage()
{
	//Now you have finished with the image, you have to release it in the V.I.M.
	try
	{
		camera->call<int>( "releaseImage", GVM_name );
	}catch( ALError& e)
	{
		std::cout<<"could not call the releaseImage method of the ALVideoDevice module"<<std::endl;
	}
}

void ImageProvider::unregisterCamera()
{
	if(cameraActive)
	{
		try
		{
			camera->callVoid("unregister", GVM_name);
		}catch (ALError &e) {
			std::cout<<"Could not call the unregister method of the ALVideoDevice, "<<e.toString().c_str()<<std::endl;
		}
	}
}


void ImageProvider::update(TJImage* theTJImage)
{
	unsigned getImageTime=SystemCall::getCurrentSystemTime();
	waitForImage(theTJImage);
	releaseImage();///////////////////why??????
	unsigned ImageTime=SystemCall::getTimeSince(getImageTime);
	//std::cout<<"imageprovider---------------"<<ImageTime<<std::endl;

#ifdef SAVE_PIC
//std::cout<<"ImageProvider::SAVE---------------------"<<std::endl;

	if (diskfull==false&&theKeyStates->numChestClick==2)
	{
		LEDRequest::getinstance()->led_on(LEDRequest::FaceLeds);
		/*theKeyStates->numChestClick=0;*/
		char m_NetPicName[30];
		static int Picindex =0;
		std::sprintf(m_NetPicName, "/home/nao/naoqi/Config/RawH%03d.raw",Picindex);
		std::cout<<m_NetPicName<<std::endl;
		std::cout<<"Picindex"<<Picindex<<std::endl;
		FILE* streamFile = fopen(m_NetPicName,"wb");
		if(streamFile==NULL)
		{
			std::cout<<"Can't open The File."<<std::endl;
		}
		else
		{
			if(fwrite(&theTJImage->unCorrectedRaw,1,FULL_IMAGE_WIDTH*FULL_IMAGE_HEIGHT*3,streamFile)==0)
			{
				diskfull=true;
			}
			/*******************************************/
			std::cout<<"sizeof(double)"<<sizeof(double)<<std::endl;
			for (int i=0;i<SafeJointData::numOfJoints;i++)
			{
				fwrite(&(theSafeJointData->angles[i]),1,sizeof(double),streamFile);
			}
			fwrite(&(theSafeJointData->timeStamp),1,sizeof(unsigned),streamFile);
			/********************************************/
			for (int i=0;i<SafeSensorData::numOfSensors;i++)
			{
				fwrite(&(theSafeSensorData->data[i]),1,sizeof(float),streamFile);
			}
			for(int i = 0; i < SafeJointData::numOfJoints; ++i)
			{
				fwrite(&(theSafeSensorData->loads[i]),1,sizeof(float),streamFile);
				fwrite(&(theSafeSensorData->temperatures[i]),1,sizeof(float),streamFile);
			}
			fwrite(&(theSafeSensorData->timeStamp),1,sizeof(unsigned),streamFile);
			/*******************************************/
// 			fwrite((char*)theSafeJointData,1,sizeof(SafeJointData),streamFile);
// 			fwrite((char*)theSafeSensorData,1,sizeof(SafeSensorData),streamFile);
			fflush(streamFile);
			fclose(streamFile);
			Picindex++;
		}
		LEDRequest::getinstance()->led_off(LEDRequest::FaceLeds);
	}

#endif




}

void ImageProvider::waitForImage(TJImage* theTJImage)
{
	if(theCameraConfig->CamChanged==true)
	{
      if (theCameraConfig->usingTopCam==true)
      {
		  camera->callVoid( "setParam", kCameraSelectID,1);
          theCameraConfig->setCamChangedFalse();
      }
	  else
	  {
          camera->callVoid( "setParam", kCameraSelectID,0);
		  theCameraConfig->setCamChangedFalse();
	  }

	}
	//if (theCameraConfig->usingTopCamAgain==true)
	//{
	//	camera->callVoid("setParam",kCameraSelectID,0);
	//	theCameraConfig->setTopCamAgainFalse();
	//}
	ALVisionImage* image;
	image = (ALVisionImage*)(camera->call<void*>("getImageLocal",GVM_name));
	//std::cout<<"here we are in  ImageProvider::waitForImage and getImageLocal "<<endl;
	// You can get some information about the image.
	int width = image->fWidth;
	int height = image->fHeight;
	int nbLayers = image->fNbLayers;
	int colorSpace = image->fColorSpace;
	long long timeStamp = image->fTimeStamp;
	timeS = (unsigned)timeStamp;
	//std::cout<<"image"<<timeS<<std::endl;
	//std::cout<<"timeStamp is "<<timeS<<endl;
	//You can get the pointer to the image data.
	const unsigned char *data;
	data = NULL;
	data = image->getData();
    unsigned int imagesize=image->getSize();
	memcpy(theTJImage->unCorrectedRaw,data,imagesize);

   //delete data;
// #ifdef TJClassfiedPic
// 	//	//std::cout<<"ImageProvider::update , genclassfiedpic "<<std::endl;
// 	//	classifiedPic=(char*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH);
// 	//	memset(classifiedPic,0,IMAGE_HEIGHT*IMAGE_WIDTH);
// 	CommandData cmd;
//
// 	if (selfMessageQueue->SearchMyMessage(idCmd,idNetworkThread,idVisionThread,cmd,sizeof(CommandData))&&cmd.RC_COMMAND==CPLANE_SEND)
// 	{
// 		theTJImage->genclassifiedPic(*theTJImage);
// 	}
// 	//    outTJImage();
// 	//	//std::cout<<"ImageProvider,the classifiedPic 1000 is "<<(int)classifiedPic[1000]<<std::endl;
// 	//	//std::cout<<"ImageProvider,the classifiedPic 2000 is "<<(int)classifiedPic[2000]<<std::endl;
// 	//	free(classifiedPic);
// 	//	//std::cout<<"ImageProvider::update++++++++ free classifiedPic"<<std::endl;
// #endif
}

void ImageProvider::registerFastAcess()
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

void ImageProvider::initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker)
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
void ImageProvider::getSensor(SafeJointData*inSafeJointData)
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
void ImageProvider::update(SafeJointData*theSafeJointData)
{
	//unsigned timeofsafejointdata=SystemCall::getCurrentSystemTime();
//     std::cout<<"ImageProvider::update(SafeJointData*theSafeJointData)"<<std::endl;
	if(true){
		getSensor(theSafeJointData);
	}
	//else{
	//	for(int i = 0; i < JointData::numOfJoints; ++i)
	//	{
	//		theSensoryJointData->angles[i] = theJointCmd->angles[i];
	//	}
	//}
	//unsigned timeforjoint=SystemCall::getTimeSince(timeofsafejointdata);
	//std::cout<<"timeof joint"<<timeforjoint<<std::endl;
}
void ImageProvider::update(SafeSensorData *theSafeSensorData)
{
	//unsigned timeforsensor=SystemCall::getCurrentSystemTime();
// 	std::cout<<"ImageProvider::update(SafeSensorData *theSafeSensorData)"<<std::endl;
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
		for(int j = 0; j < KeyStates::numberOfKeys; ++j)
			keyStates.pressed[j] = sensorValues[i++] != 0;

		//click the number of pressing button

		chestButton->updateFrame(keyStates.pressed[KeyStates::ChestButton]);
		leftFootButton->updateFrame((keyStates.pressed[KeyStates::LBumperLeft]||keyStates.pressed[KeyStates::LBumperRight]));
		rightFootButton->updateFrame((keyStates.pressed[KeyStates::RBumperLeft]||keyStates.pressed[KeyStates::RBumperRight]));
		keyStates.numChestClick = chestButton->getNumClicks();
		keyStates.numLeftClick = leftFootButton->getNumClicks();
		keyStates.numRightClick = rightFootButton->getNumClicks();

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
	//unsigned timeforsensor1=SystemCall::getTimeSince(timeforsensor);
	//std::cout<<"timeforsensor-------------"<<timeforsensor1<<std::endl;
	////////////////////////////////////////
// 	int time=dcm->getTime(0);
// 	std::cout<<"dcm->getTime"<<time<<std::endl;
}
void ImageProvider::update(FrameInfo *theFrameInfo)
{
	theFrameInfo->time = timeS;
}
void ImageProvider::update(KeyStates *theKeyStates)
{
	*theKeyStates = this->keyStates;
}
void ImageProvider::update(OdometryData*theOdometryData)
{
	selfMessageQueue->SearchMyMessage(idOdometryData,idActuatorThread,idRobotThread,
		(char*)theOdometryData,sizeof(OdometryData));
}
MAKE_KS(ImageProvider)
