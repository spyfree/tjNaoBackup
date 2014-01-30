#ifndef __CameraMatrix_h_
#define __CameraMatrix_h_
#include "Tools/Math/Pose3D.h"

class CameraMatrix : public Pose3D
{
public:
	CameraMatrix(const Pose3D& pose3D):Pose3D(pose3D), isValid(true){}
	CameraMatrix():isValid(true){}
public:
	bool isValid;
};

//class CameraTopMatrix : public CameraMatrix{};
//class CameraMatrix: public CameraMatrix{};
#endif
