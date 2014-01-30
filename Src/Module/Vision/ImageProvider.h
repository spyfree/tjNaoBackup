#ifndef ImageProvider_H
#define ImageProvider_H

#include "Module/Vision/TJImage.h"
#include "Module/Common/FrameInfo.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/KeyStates.h"
#include "Module/Common/VisionHeader.h"
#include "alptr.h"
#include "albroker.h"
#include "alvisiondefinitions.h"
#include "almodule.h"
#include "alproxy.h"
#include "alimage.h"
#include "almemoryfastaccess.h"
#include "Module/Common/LEDRequest.h"
#include "Module/Common/ClickButton.h"
#include "Module/Representations/MotionControl/OdometryData.h"
// #include "dcmproxy.h"

DEFINE_KS(ImageProvider)
REQUIRES(CameraConfig)
PROVIDES(SafeJointData)
PROVIDES(SafeSensorData)
PROVIDES(KeyStates)
REQUIRES(SafeJointData)
REQUIRES(SafeSensorData)
REQUIRES(KeyStates)
PROVIDES(TJImage)
PROVIDES(FrameInfo)
PROVIDES(OdometryData)
USES(FrameInfo)
USES(TJImage)
//USES(CameraConfig)
END_KS


namespace AL
{
	class ALProxy;
	class ALValue;
	class ALVisionImage;
	//class ALPtr;
};

using namespace AL;

class ImageProvider: public ImageProviderBase,BaseProvider
{
public:
	ImageProvider();
	~ImageProvider();
	void update(TJImage* theTJImage);
	void update(SafeJointData* theSafeJointData);
	void update(SafeSensorData *theSafeSensorData);
	void update(FrameInfo* theFrameInfo);
	void update(KeyStates *theKeyStates);
	void inCameraConfig(){}
	void inSafeJointData(){}
	void inSafeSensorData(){}
	void inKeyStates(){}
	void outKeyStates(){};
	void outTJImage(){};
	void outSafeJointData(){}
	void outSafeSensorData(){}
	void outFrameInfo(){}
	void update(OdometryData*theOdometryData);
	void outOdometryData(){}
private:
	void initCamera();
	ALPtr<ALProxy> camera;
// 	ALPtr<ALProxy> dcm;///////////////////////////////
	std::string GVM_name;

	bool cameraActive;

	//int resolution;
	//int colorspace;
	//int fps;
	//int cameraID;

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

	void registerCamera();
    //void readConfig();
    void setCamera();
	void waitForImage(TJImage* theTJImage);
	void releaseImage();
	void unregisterCamera();

	//------------use for sensor -----------//
	ALPtr<ALMemoryFastAccess> alfastaccessjoints;
	ALPtr<ALMemoryFastAccess> alfastaccessSensors;
	SensorData sensorData;
	SensoryJointData sensoryJointData;
	std::vector<float> jointangles;
	std::vector<float> sensorValues;
	KeyStates keyStates;/**< State of the Buttons*/
	SmartPtr<ClickButton> chestButton,
		leftFootButton,
		rightFootButton;
	void registerFastAcess();
	void initSyncWithALMemory(AL::ALPtr<AL::ALBroker> broker);
	void getSensor(SafeJointData*inSafeJointData);

	//char *classifiedPic;//[IMAGE_HEIGHT*IMAGE_WIDTH];
	unsigned timeS;

//////////////////////////////////////////
};
#endif
