#ifndef ODEData_H
#define ODEData_H
#include "../../ODESimulation/SimulationCore/Tools/SimMath.h"
#include "Platform/SimuQt/RoboCupCtrl.h"
#include "Simulation/ODESimulation/SimulationCore/Simulation/SimObject.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/Geometry.h"
#include "Tools/Math/Common.h"
#include "Tools/Math/Vector2.h"
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/MotionControl/OdometryData.h"
#include "Module/Common/NaoStructure.h"
DEFINE_KS(ODEData)
REQUIRES(CameraMatrix)
PROVIDES(SensoryJointData)
USES(SensoryJointData)
PROVIDES(SafeSensorData)
PROVIDES(OdometryData)
PROVIDES(NaoStructure)
//REQUIRES(CameraMatrix)
END_KS

class ODEData:public ODEDataBase,public BaseProvider
{
public:
	ODEData();
	~ODEData();
	void GetData();
	RoboCupCtrl* roboCupCtrl;
	void inCameraMatrix(){}
	void update(OdometryData* theOdometryData);
	void outOdometryData(){}
	void update(SensoryJointData* theSensoryJointData);
	void outSensoryJointData(){}
	void update(SafeSensorData *theSafeSensorData);
	void outSafeSensorData(){}
	void update(NaoStructure *theNaoStructure);
	void outNaoStructure(){};
	//void inCameraBottomMatrix(){}

protected:
	SimObject* ball;
	SimObject* robot;//robot position
	SimObject* camera;//camera position

private:
	bool initOdome;
	double OdomeInitX,OdomeInitY,OdomeInitRot;
	double OdomeX,OdomeY,OdomeRot;
	int ballCenterX;
	int ballCenterY;
	double ballXData;
	double ballYData;
	double robotXData;
	double robotYData;
	double distToBallData;
	double direcToBallData;
	double direct;
	bool confidence;
	bool goalCfd;
	int goalcolor;
	double regforleft;
	double regforight;
	double cameraX;
	double cameraY;
	double cameraZ;
	JointData jointData;
	SensorData sensorData;
	//CameraMatrix4X3 cameraMatrix;
	//Matrix4x4 cameraMatrix4;//wang
	Vector2<double> pointOnField1,pointOnField2,pointOnField3,pointOnField4;//1:leftFar 2:leftClose 3:rightClose 4:rightClose
	Matrix3d robotRotation;
	Vector3d direction;//wang
	

};
#endif
