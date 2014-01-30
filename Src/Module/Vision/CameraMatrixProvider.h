#ifndef __CameraMatrixProvider_h_
#define __CameraMatrixProvider_h_
#include "Module/Representations/KS.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Common/SensorData.h"
#include "Tools/TJAssign.h"
#include "Module/Vision/CameraMatrixCalibration.h"
DEFINE_KS(CameraMatrixProvider)
REQUIRES(CameraConfig)
REQUIRES(NaoStructure)
REQUIRES(SafeSensorData)
PROVIDES(CameraMatrix)
//PROVIDES(CameraMatrix)
END_KS

/**
* A Class provdes camera matrix relative to robot coordinate
* Robot Coordinate: Vertical Projection of Body origin onto the ground
*/
class CameraMatrixProvider: public CameraMatrixProviderBase
{
public:
	CameraMatrixProvider();
	~CameraMatrixProvider(){}
private:
	//void update(CameraTopMatrix* theCameraTopMatrix);
	//void update(CameraMatrix* theCameraBottomMatrix);
	void update(CameraMatrix* theCameraMatrix);
	void inCameraConfig();
	void inNaoStructure();
	void inSafeSensorData();
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
public:
	class Parameters:public paramAssign
	{
	public:
		void massAssign();
	public:
		double rr;
		double bb;
		double zz;
	};
	Parameters params;
};

#endif