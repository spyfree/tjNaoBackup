#ifndef __SensorComm_h_DEFINED
#define __SensorComm_h_DEFINED

#include "Module/Vision/TJImage.h"
#include "Module/Network/RoboCupGameControlData.h"
#include "Module/Common/Geometry.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Vision/ScanVision.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Vision/Regionpercept.h"
#include "Module/Vision/CameraMatrixCalibration.h"
DEFINE_KS(SensorComm)
REQUIRES(TJImage)
//REQUIRES(CameraConfig)
//REQUIRES(CameraTopMatrix)
//REQUIRES(CameraMatrix)
REQUIRES(CameraMatrix)
PROVIDES(Regionpercept)
//REQUIRES(RoboCupGameControlData)
//PROVIDES(OdometryData)
END_KS

class SensorComm : public SensorCommBase,BaseProvider
{
public:
	SensorComm();
	~SensorComm();


	ScanVision* TJScanVision;
	//Matrix4x4 cameraMatrix;
	//CameraMatrix4X3 cameraMatrixScanvision;
	//TJImage imagetj;
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
	void update(Regionpercept* theRegionpercept);
	void inTJImage();
	//void inCameraConfig();
	//void inCameraTopMatrix();
	//void inCameraBottomMatrix();
    void inCameraMatrix();
	//void update(OdometryData* theOdometryData);
	void outRegionpercept(){}
	//void outOdometryData(){}
	//void outVisionInterface();

	//void sendVisionResult();

	//========================================
private:
	Geometry::GTLine GenerateHorizon();
private:
	Geometry::GTLine horizonLine;
};
#endif//__SensorComm_h_DEFINED
