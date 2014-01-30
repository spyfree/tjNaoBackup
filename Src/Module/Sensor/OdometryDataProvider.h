#ifndef __OdometryDataProvider_h_
#define __OdometryDataProvider_h_

#include "Module/Representations/KS.h"
#include "Module/Common/NaoStructure.h"
#include "Module/Common/SensorData.h"
//#include "Module/Common/MotionCmd.h"
//#include "Module/Representations/MotionControl/InertiaMatrix.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Tools/Math/Pose3D.h"
#include "Tools/Math/Matrix.h"
#include "Module/Representations/BaseProvider.h"

DEFINE_KS(OdometryDataProvider)
REQUIRES(NaoStructure)
REQUIRES(SafeSensorData)
//测试：根据动作命令校正
//REQUIRES(MotionPlan)
//PROVIDES(InertiaMatrix)
PROVIDES(OdometryData)
END_KS

class OdometryDataProvider:public OdometryDataProviderBase, BaseProvider
{
public:
	OdometryDataProvider();
	~OdometryDataProvider(){}
private:
	//void update(InertiaMatrix* theInertiaMatrix);
	void update(OdometryData* theOdometryData);
	void inNaoStructure();
	void inSafeSensorData();
	//void inMotionPlan();
	//void outInertiaMatrix();
	void outOdometryData();

private:
 
	/*Vector3<double> lastBodyPosition;
	Vector3<double> initlastBodyPos;
	Pose3D lastBodyProjection;
	Pose3D lastBodyOffset;
	Vector3<double> BodyOffset;
	Vector3<double> SpanOffset;*/
	//Pose3D BodyOffset;
	int lastsupmod;
    RotationMatrix rsupfoot2world;
	RotationMatrix lastrsupfoot2world;
	Vector2<double> test;

	RotationMatrix lastrBody2Supfoot;
	Pose3D lastInertiaMatrix;
	Pose3D lastSwingFoot;
	double lastBody2SupZAngle;
	Vector3<double> lastFootSpan;
	Vector3<double> SpanOffset;
	Vector3<double> lastBodyPosition;
	//InertiaMatrix tmpInertiaMatrix;
    RotationMatrix lastrSwg2Supfoot;
	bool flag;

	double mmm;

	int dataDebugcounter;

}; 


#endif
