#include "CameraConfig.h"
#include "Platform/GetSetting.h"
#include <iostream>
#include <fstream>
#include <string>
CameraConfig::CameraConfig()
{
	readCameraConfig();
}

void CameraConfig::readCameraConfig()
{
	std::string buffer;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/cameraSetting.cfg";// TODO:read config file can not be done everytime ,need to change
	//std::cout<<path<<std::endl;
	std::ifstream stream(path.c_str());
	if (!stream.is_open())
	{
		std::cout<<path<<" Open camera configure file failed "<<std::endl;
	}
	const int cfgLine = 32;
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
	DEFAULT_CAMERA_WHITEBALANCE=config[15];
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_WHITEBALANCE is "<<DEFAULT_CAMERA_WHITEBALANCE<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_GAIN is "<<DEFAULT_CAMERA_GAIN<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_BRIGHTNESS is "<<DEFAULT_CAMERA_BRIGHTNESS<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_CONTRAST is "<<DEFAULT_CAMERA_CONTRAST<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_SATURATION is "<<DEFAULT_CAMERA_SATURATION<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_HUE is "<<DEFAULT_CAMERA_HUE<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_AUTO_EXPOSITION is "<<DEFAULT_CAMERA_AUTO_EXPOSITION<<std::endl;
	std::cout<<"in CameraConfig, DEFAULT_CAMERA_EXPOSURE is "<<DEFAULT_CAMERA_EXPOSURE<<std::endl;

	if (cameraID == 0)
	{
		usingTopCam=true;
	}
	if (cameraID == 1)
	{
		usingTopCam=false;
	}
	CamChanged = false;
}

// void CameraConfig::setTopCamAgainFalse()
//{
//	usingTopCamAgain=false;
//}
// void CameraConfig::setTopCamfalse()
//{
//	usingTopCam = false;
//}
