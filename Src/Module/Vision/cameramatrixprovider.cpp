#include "CameraMatrixProvider.h"
#include "Tools/Math/Common.h"
#include "Module/Common/NaoConfig.h"
#include <iostream>
#include "Platform/GetSetting.h"
#include <fstream>
#include <string>
CameraMatrixProvider::CameraMatrixProvider():
	CameraTopOffsetX(58.71 / 1000),//53.9
	CameraTopOffsetY(0.0), 
	CameraTopOffsetZ(63.64 / 1000), //67.9
	CameraTopRotY(0.0), 
	CameraTopOpenAngle(deg2rad(47.64)),//34.8
	CameraBottomOffsetX(50.71 / 1000),//48.8
	CameraBottomOffsetY(0.0),
	CameraBottomOffsetZ(17.74 / 1000),//23.81
	CameraBottomRotY(deg2rad(39.7)),//40
	CameraBottomOpenAngle(deg2rad(47.64))//34.8
{
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/CameraCalibration.cfg";
	//paramAssign::param paramMap;
	params.readCfgFile(path);
	params.massAssign();
}
// TODO: Set the other camera matrix's isValid to false if use current camera
void CameraMatrixProvider::update(CameraMatrix* theCameraMatrix)
{  
#ifdef CameraMatrixCalibration
#ifndef NAO_DEBUG 
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/CameraCalibration.cfg";
#else
	std::cout<<"sim cameraMatrixCalibration"<<std::endl;
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/SimCameraCalibration.cfg";
#endif
	//paramAssign::param paramMap;
	params.readCfgFile(path);
	params.massAssign();
	params.paramReset();
#endif
	if(theCameraConfig->usingTopCam)
    {
		int supfootLink = 0;
	    RotationMatrix RHeadW;
	    RotationMatrix RobotRotationW;
	   RobotRotationW.rotateX(theNaoStructure->supportFootRotWorld.x).rotateY(theNaoStructure->supportFootRotWorld.y).rotateZ(theNaoStructure->supportFootRotWorld.z);
	if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
	{
		supfootLink = NaoStructure::lFootLink;
	}else
	{
		supfootLink = NaoStructure::rFootLink;
	}
	//RotationMatrix test = theNaoStructure->uLink[supfootLink].R.invert();
	RHeadW = /*RobotRotationW * */(theNaoStructure->uLink[NaoStructure::headEndLink].R * theNaoStructure->uLink[supfootLink].R.invert()); 
	//double rx = RHeadW.getXAngle();
	//double ry = RHeadW.getYAngle();
	//double rz = RHeadW.getZAngle();
	 //rx = theNaoStructure->uLink[NaoStructure::headEndLink].R.getXAngle();
	 //ry = theNaoStructure->uLink[NaoStructure::headEndLink].R.getYAngle();
	 //rz = theNaoStructure->uLink[NaoStructure::headEndLink].R.getZAngle();
	theCameraMatrix->translation = RHeadW * Vector3<double>(CameraTopOffsetX, CameraTopOffsetY, CameraTopOffsetZ)
																				+RobotRotationW * theNaoStructure->uLink[NaoStructure::headEndLink].p ;
	//now camera pos relative to support foot, translate to pos relative to robot coordinate
	Vector3<double> offset = (theNaoStructure->uLink[NaoStructure::bodyLink].p -theNaoStructure->uLink[supfootLink].p);
	offset.z = 0;
	theCameraMatrix->translation -= offset;
	theCameraMatrix->translation.x=theCameraMatrix->translation.x*1000;
	theCameraMatrix->translation.y=theCameraMatrix->translation.y*1000;
	theCameraMatrix->translation.z=theCameraMatrix->translation.z*1000;
	theCameraMatrix->rotation = RHeadW.rotateY(CameraTopRotY);
	theCameraMatrix->isValid = true;
   }
	if(!theCameraConfig->usingTopCam)
	{
// 		int supfootLink = 0;
// 		RotationMatrix RHeadW;
// 		RotationMatrix RobotRotationW;
// 		RobotRotationW.rotateX(theNaoStructure->supportFootRotWorld.x).rotateY(theNaoStructure->supportFootRotWorld.y).rotateZ(theNaoStructure->supportFootRotWorld.z);
// 		if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
// 		{
// 			supfootLink = NaoStructure::lFootLink;
// 		}else
// 		{
// 			supfootLink = NaoStructure::rFootLink;
// 		}
// 		RHeadW = /*RobotRotationW **/ (theNaoStructure->uLink[NaoStructure::headEndLink].R * theNaoStructure->uLink[supfootLink].R.invert()); 
// 		theCameraMatrix->translation = RHeadW * Vector3<double>(CameraBottomOffsetX, CameraBottomOffsetY, CameraBottomOffsetZ)
// 			+ RobotRotationW * theNaoStructure->uLink[NaoStructure::headEndLink].p ;
// 		//now camera pos relative to support foot, translate to pos relative to robot coordinate
// 		Vector3<double> offset = (theNaoStructure->uLink[NaoStructure::bodyLink].p -theNaoStructure->uLink[supfootLink].p);
// 		offset.z = 0;
// 		theCameraMatrix->translation -= offset;
// 		theCameraMatrix->translation.x=theCameraMatrix->translation.x*1000;
// 		theCameraMatrix->translation.y=theCameraMatrix->translation.y*1000;
// 		theCameraMatrix->translation.z=theCameraMatrix->translation.z*1000;//视觉的单位为mm
// 		theCameraMatrix->rotation = RHeadW.rotateY(CameraBottomRotY);
// 		theCameraMatrix->isValid = true;

		/*SafeSensorData sensor;*/
		////The Original Method 1
// 		selfMessageQueue->SearchMyMessage(idSimSensorData,idActuatorThread, idBehaviorThread,(char*)&sensor
// 			,sizeof(SensorData));
		int supfootLink = 0;
		RotationMatrix RHeadW;
		double bRoll = theSafeSensorData->data[SensorData::angleX];
		double bTilt = theSafeSensorData->data[SensorData::angleY];
		//std::cout<<"!!!!!!!bodyRoll is "<<bRoll*180/3.1415<<std::endl;
		//std::cout<<"!!!!!!!bodyTilt is "<<bTilt*180/3.1415<<std::endl;
		RotationMatrix Rbw = RotationMatrix().rotateX(bRoll+params.rr).rotateY(bTilt+params.bb);
		RotationMatrix Rfw;//rotation of supporting foot in world cooridnate
		if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		{
			Rfw = theNaoStructure->uLink[NaoStructure::bodyLink].R.invert()/**theNaoStructure->uLink[NaoStructure::lFootLink].R */ ;
		}else
		{
			Rfw =  theNaoStructure->uLink[NaoStructure::bodyLink].R.invert()/**theNaoStructure->uLink[NaoStructure::rFootLink].R */ ;
		}
		Rfw = Rbw*Rfw;
		double angleZ = Rfw.getZAngle();
		// 		RotationMatrix RobotRotationW;
		// 		RobotRotationW.rotateZ(angleZ);


		if(theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_LEFT || theNaoStructure->supportingMode == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		{
			supfootLink = NaoStructure::lFootLink;
		}else
		{
			supfootLink = NaoStructure::rFootLink;
		}
		RHeadW = Rfw * (theNaoStructure->uLink[NaoStructure::headEndLink].R); 
		theCameraMatrix->translation = RHeadW * Vector3<double>(CameraBottomOffsetX, CameraBottomOffsetY, CameraBottomOffsetZ)
			+ Rfw * theNaoStructure->uLink[NaoStructure::headEndLink].p ;
		//now camera pos relative to support foot, translate to pos relative to robot coordinate
		Vector3<double> offset = Rfw*(theNaoStructure->uLink[NaoStructure::bodyLink].p -theNaoStructure->uLink[supfootLink].p);
		offset.z = 0;
		theCameraMatrix->translation -= offset;
		theCameraMatrix->translation.x=theCameraMatrix->translation.x*1000;
		theCameraMatrix->translation.y=theCameraMatrix->translation.y*1000;
		theCameraMatrix->translation.z=theCameraMatrix->translation.z*1000;//视觉的单位为mm
		theCameraMatrix->rotation = RHeadW.rotateY(CameraBottomRotY).rotateZ(params.zz);
		theCameraMatrix->isValid = true;
	}
}

void CameraMatrixProvider::inNaoStructure()
{

}
void CameraMatrixProvider::inCameraConfig()
{

}  
void CameraMatrixProvider::outCameraMatrix()
{

}
void CameraMatrixProvider::inSafeSensorData()
{

}
void CameraMatrixProvider::Parameters::massAssign()
{
	ASSIGN(rr);
	ASSIGN(bb);
	ASSIGN(zz);
}

MAKE_KS(CameraMatrixProvider)