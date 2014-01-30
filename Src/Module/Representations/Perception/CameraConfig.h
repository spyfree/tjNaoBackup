#ifndef __CameraConfig_h_
#define __CameraConfig_h_

class CameraConfig
{
public:
	CameraConfig();

	int resolution;
	int colorspace;
	int fps;
	int cameraID;//CameraID 0 is the top camera,1 is the bottom camera

	// Color Settings
	int DEFAULT_CAMERA_AUTO_GAIN;// = 0; // AUTO GAIN OFF
	int DEFAULT_CAMERA_GAIN;// = 4;
	int DEFAULT_CAMERA_AUTO_WHITEBALANCE;// = 0; // AUTO WB OFF
	int DEFAULT_CAMERA_BLUECHROMA;// = 118;//158; //131;
	int DEFAULT_CAMERA_REDCHROMA;// = 116;//68; //72;
	int DEFAULT_CAMERA_BRIGHTNESS;// = 128;
	int DEFAULT_CAMERA_CONTRAST;// = 64;
	int DEFAULT_CAMERA_SATURATION;// = 128;
	int DEFAULT_CAMERA_HUE;// = 0;
	// Exposure length
	int DEFAULT_CAMERA_AUTO_EXPOSITION;// = 0; // AUTO EXPOSURE ON
	int DEFAULT_CAMERA_EXPOSURE;// = 300;
	int DEFAULT_CAMERA_WHITEBALANCE;//-45

	mutable bool usingTopCam;
	mutable bool CamChanged;
public:
	void readCameraConfig();
    void setCamChangedFalse()const{ CamChanged = false;}

	//these three functions are used by behavior 
	void setTopCamfalse() const { usingTopCam = false; CamChanged=true;}
	void setTopCamtrue() const { usingTopCam = true; CamChanged=true;}
	const bool getusingTopCam() const { return usingTopCam; }
    
};
#endif
