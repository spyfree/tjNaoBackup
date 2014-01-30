/* simulation the function of CameraMatrixProvider ----wang*/
#ifndef ODECameraProvider_H
#define ODECameraProvider_H
#include "Module/Representations/KS.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/CameraConfig.h"
DEFINE_KS(ODECameraProvider)
REQUIRES(NaoStructure)
REQUIRES(CameraConfig)
PROVIDES(CameraMatrix)
//PROVIDES(CameraTopMatrix)
//PROVIDES(CameraMatrix)
END_KS
class ODECameraProvider: public ODECameraProviderBase
{
public:
	ODECameraProvider();
	~ODECameraProvider(){}
private:
	//void update(CameraTopMatrix* theCameraTopMatrix);
	//void update(CameraMatrix* theCameraBottomMatrix);
	void inNaoStructure();
	void inCameraConfig();
	void update(CameraMatrix* theCameraMatrix);
	void outCameraMatrix();
	//void outCameraTopMatrix();
	//void outCameraBottomMatrix();
private:
	// TODO: move them into proper Camera Information Provider class
	const double CameraTopOffsetX, CameraTopOffsetY, CameraTopOffsetZ;
	const double CameraTopRotY;
	const double CameraTopOpenAngle;
	const double CameraBottomOffsetX, CameraBottomOffsetY, CameraBottomOffsetZ;
	const double CameraBottomRotY;
	const double CameraBottomOpenAngle;
};

#endif