// @to build a part of the body for parameters testing  (目前只做sensors)
// @ from LocalRobot

#ifndef __LocalPart_h_
#define __LocalPart_h_
//#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/RobotState.h"
#include "Platform/SimuQt/RoboCupCtrl.h"
#include "Module/Representations/BaseProvider.h"
#include <Simulation/SimObject.h>	//x
#include "Module/Vision/TJImage.h"

class LocalPart : public BaseProvider
{
public:
	LocalPart();
	LocalPart(SimObject* obj);

	~LocalPart();

private:
//joints(useless)
	//int spJoints[JointData::numOfJoints], /**< The handles to the sensor ports of the joints. */  
	//	 apJoints[JointData::numOfJoints], /**< The handles to the actuator ports of the joints. */
	//	 psJoints[JointData::numOfJoints];/**< The handles to the power switches of the joints. */
//sensors   
	int spCamera;
	int spAcc;
	int spGyro;
	int spRFsrFL, spRFsrFR, spRFsrBL, spRFsrBR, spLFsrFL, spLFsrFR, spLFsrBL, spLFsrBR;
	//x
	int myFsr;

	SensorData sensorData;

//(useless)
	/*JointCmd jointRequest;
	JointCmd MotionBoardJointRequest;	 
	JointData jointData;*/

//
	RoboCupCtrl* ctrl;
	

public:
	//only update the sensors recently
	void update();
	SensorData getSensorData();

private:
	//static const char* getJointName(JointData::Joint joint);
};
#endif
