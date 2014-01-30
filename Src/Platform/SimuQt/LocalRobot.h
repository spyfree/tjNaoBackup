#ifndef __LocalRobot_h_
#define __LocalRobot_h_
#include "Module/Common/JointData.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/RobotState.h"
#include "RoboCupCtrl.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/FrameInfo.h"
#include <Simulation/SimObject.h>	//x
#include "Module/Vision/TJImage.h"

class LocalRobot : public BaseProvider
{

private:
	int spJoints[JointData::numOfJoints], /**< The handles to the sensor ports of the joints. */  
		apJoints[JointData::numOfJoints], /**< The handles to the actuator ports of the joints. */
		psJoints[JointData::numOfJoints];/**< The handles to the power switches of the joints. */

	int spCamera;
	int spAcc;
	int spGyro;
	int spRFsrFL, spRFsrFR, spRFsrBL, spRFsrBR, spLFsrFL, spLFsrFR, spLFsrBL, spLFsrBR;

	static bool naoV3; 
	SimObject* me; 
	SimObject* rightFoot; 

	//Image image;
	unsigned imageTimeStamp, /**< The theoretical timestamp of the last calculated image. */
		imageLastTimeStampSent; /**< The timestamp of the last sent image. */
	unsigned char* imageReading;
	FrameInfo frameInfo;

	//int apLeds[LEDRE]
	JointCmd jointRequest;
	JointCmd MotionBoardJointRequest;	 
	JointData jointData;

	SensorData sensorData;
	SimObject* objRobot;
	SimObject* objBody;
	RoboCupCtrl* ctrl;

	unsigned char image[IMAGE_HEIGHT][IMAGE_WIDTH][3];


public:
	LocalRobot(SimObject* obj);
	~LocalRobot();
	//virtual int main();
	void update();
	void setFallState(bool fall, double dx, double dy, double time);
	void setFallState(bool fall){robotState.fallState.fall = fall;}
	void setUpdateModleFlag(bool update){robotState.fallState.updateModel = update;}
	void setRobotRecoverd(bool recovered);
	void getImage(unsigned char* src);
	//void saveRGBlog(unsigned char* src);
	//void saveRawPic();
private:
	static const char* getJointName(JointData::Joint joint);
	RobotState robotState;

	//x
public:
	static double  bugJointsData[JointData::numOfJoints];	//for debug
	static int myApJoints[JointData::numOfJoints]; /**< The handles to the actuator ports of the joints. */
	static int mySpJoints[JointData::numOfJoints];
	static SensorData mySensorData;
public:
	static bool	 saToolsSwitch;
public:
	SimObject* objOfThisRobot;
};
#endif