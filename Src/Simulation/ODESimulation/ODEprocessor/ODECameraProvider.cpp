#include "ODECameraProvider.h"
#include "Tools/Math/Common.h"
#include "Module/Common/NaoConfig.h"
#include <iostream>
ODECameraProvider::ODECameraProvider():
CameraTopOffsetX(58.71 / 1000),
CameraTopOffsetY(0.0), 
CameraTopOffsetZ(63.64 / 1000), 
CameraTopRotY(0.0), 
CameraTopOpenAngle(deg2rad(47.64)),
CameraBottomOffsetX(50.71 / 1000),
CameraBottomOffsetY(0.0),
CameraBottomOffsetZ(17.74 / 1000),
CameraBottomRotY(deg2rad(39.7)),
CameraBottomOpenAngle(deg2rad(47.64))
{
	std::cout<<"end of ode cam"<<std::endl;
}
// TODO: Set the other camera matrix's isValid to false if use current camera
void ODECameraProvider::update(CameraMatrix* theCameraMatrix)
{
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
		RHeadW = /*RobotRotationW **/ (theNaoStructure->uLink[NaoStructure::headEndLink].R * theNaoStructure->uLink[supfootLink].R.invert()); 
		theCameraMatrix->translation = RHeadW * Vector3<double>(CameraBottomOffsetX, CameraBottomOffsetY, CameraBottomOffsetZ)
			+ RobotRotationW * theNaoStructure->uLink[NaoStructure::headEndLink].p ;
		//now camera pos relative to support foot, translate to pos relative to robot coordinate
		Vector3<double> offset = (theNaoStructure->uLink[NaoStructure::bodyLink].p -theNaoStructure->uLink[supfootLink].p);
		offset.z = 0;
		theCameraMatrix->translation -= offset;
		theCameraMatrix->translation.x=theCameraMatrix->translation.x*1000;
		theCameraMatrix->translation.y=theCameraMatrix->translation.y*1000;
		theCameraMatrix->translation.z=theCameraMatrix->translation.z*1000;//视觉的单位为mm
		theCameraMatrix->rotation = RHeadW.rotateY(CameraBottomRotY);
		theCameraMatrix->isValid = true;
	}
}
void ODECameraProvider::inNaoStructure()
{

}
void ODECameraProvider::inCameraConfig()
{

}
void ODECameraProvider::outCameraMatrix()
{

}

MAKE_KS(ODECameraProvider)