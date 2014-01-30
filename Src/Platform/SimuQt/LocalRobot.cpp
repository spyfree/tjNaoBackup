#include "LocalRobot.h"
#include "Simulation/ODESimulation/SimulationCore/Simulation/APIDatatypes.h"
#include "Platform/SystemCall.h"
#include <iostream>
#include <fstream>
#include <map>
//#include "Module/Actuator/MatlabHelp/MatlabHelp.h"//x

double LocalRobot::bugJointsData[JointData::numOfJoints]={0};//for debug	
bool LocalRobot::saToolsSwitch = false;
bool	LocalRobot::naoV3=false;		
int LocalRobot::myApJoints[JointData::numOfJoints]={0};
int LocalRobot::mySpJoints[JointData::numOfJoints] = {0};
SensorData LocalRobot::mySensorData;

LocalRobot::LocalRobot(SimObject* obj):imageLastTimeStampSent(0),imageTimeStamp(0)
{//x
	saToolsSwitch = 0;

	for (int i=0;i<JointData::numOfJoints;i++)		//for debug
	{
		bugJointsData[i]=0;
	}
	//end
	objOfThisRobot = new SimObject;
	objOfThisRobot = obj;
	//end
	ctrl = RoboCupCtrl::getController();
	std::string robotName = obj->getFullName();

	me = ctrl->getObjectReference(robotName);
	me->findObject(rightFoot, "rFoot", false);
	if(!rightFoot)
		naoV3 = true;
	//me->findObject(rightFoot,"RAnkleRoll",false);
	//---------
	std::vector<std::string> parts;
	parts.resize(2);
	parts[0] = obj->getFullName();//RoboCupCtrl::getRobotFullName();
	for(int i = 1; i < JointData::numOfJoints; ++i)
	{
		std::string angleName(LocalRobot::getJointName(JointData::Joint(i)));
		parts[1] = angleName;
		spJoints[i] = ctrl->getSensorportIdByPartOfName(parts);
		mySpJoints[i] = spJoints[i];
		parts[1] += ".motorAngle";
		apJoints[i] = ctrl->getActuatorportIdByPartOfName(parts);
		myApJoints[i] = apJoints[i];
		parts[1] = angleName + ".switch";
		psJoints[i] = ctrl->getActuatorportIdByPartOfName(parts);
	}
	parts[1] = "camera";
	spCamera = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "accelerometer";
	spAcc = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "gyroscope";
	spGyro = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrFL";
	spRFsrFL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrFR";
	spRFsrFR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrBL";
	spRFsrBL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "RFsrBR";
	spRFsrBR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrFL";
	spLFsrFL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrFR";
	spLFsrFR = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrBL";
	spLFsrBL = ctrl->getSensorportIdByPartOfName(parts);
	parts[1] = "LFsrBR";
	spLFsrBR = ctrl->getSensorportIdByPartOfName(parts);
}

LocalRobot::~LocalRobot()
{
	if(imageReading)
	{
		delete []imageReading;
		imageReading = NULL;
	}
}

void LocalRobot::update()
{

	// Reciev JointCmd from Acuator Thread
	// JointCmd send @ ActuatorSimProvider -> sendJointCmd2ODESimulation()
	selfMessageQueue->SearchMyMessage(idJointCmd,idActuatorThread,idODESimulation,
		(char*)&jointRequest ,sizeof(JointCmd));
	/*MessageType type2(idMotionBoardJointCmd,idActuatorThread,idODESimulation);
	if (selfMessageQueue->IsMyMessage(type2))
	{
	memcpy(&MotionBoardJointRequest,selfMessageQueue->SearchMyMessage(type2).GetData(),sizeof(JointCmd));
	}*/
	//else if(mode == SystemCall::simulatedRobot)
	//joints

	if(saToolsSwitch)
	{
		//for (int i = 1; i< JointData::numOfJoints;i++)
		//{
		//	jointRequest.angles[i] = bugJointsData[i-1];
		//	ctrl->setActuatorport(apJoints[i],	(jointRequest.angles[i])	);
		//}
		//joints
		/*****************XuTao:update by12/21***********************/
		for(int i = 1; i < JointData::numOfJoints; ++i)
		{
			// Get angles
			double actualAngle;
			if(spJoints[i] == -1)  // if joint does not exist
				actualAngle = JointData::off;
			else
			{
				ctrl->getSensorValue(spJoints[i], actualAngle);
				jointData.angles[i] = deg2rad(actualAngle);
			}
		}
		/*****************XuTao:update by12/21***********************/
	}
	else	//@todo:	else的部分有点乱，需要检视一下
	{
		//joints
		for(int i = 1; i < JointData::numOfJoints; ++i)
		{
			// Get angles
			double actualAngle;
			if(spJoints[i] == -1)  // if joint does not exist
				actualAngle = JointData::off;
			else
			{
				ctrl->getSensorValue(spJoints[i], actualAngle);
				jointData.angles[i] = deg2rad(actualAngle);
			}

			// Set angles
			double targetAngle = jointRequest.angles[i];
			if(targetAngle != JointData::off && 
				targetAngle != JointData::ignore &&
				spJoints[i] != -1) // if joint does exist
				ctrl->setActuatorport(apJoints[i], rad2deg(targetAngle));
		}

	} // if else
	// Battery
	sensorData.data[SensorData::voltage] = 1.0f;
	// Acceleration sensors
	double* acceleration;
	ctrl->getSensorValue(spAcc, acceleration);
	sensorData.data[SensorData::accX] = float(acceleration[0]);// / 9.81);
	sensorData.data[SensorData::accY] = float(acceleration[1]);// / 9.81);
	sensorData.data[SensorData::accZ] = float(acceleration[2]);// / 9.81);

	//@todo: compare signs and absolute values with real robot
	// Gyros
	double* rotationSpeed;
	ctrl->getSensorValue(spGyro, rotationSpeed);
	sensorData.data[SensorData::gyroX] = float(rotationSpeed[0]);
	sensorData.data[SensorData::gyroY] = float(rotationSpeed[1]);
	sensorData.data[SensorData::gyroZ] = float(rotationSpeed[2]);

	//MATLABHELP_PLOT(sensorData.data[SensorData::gyroX],"myGyroX");  //x
	//MATLABHELP_PLOT(sensorData.data[SensorData::gyroY],"myGyroY");  //x
	//MATLABHELP_PLOT(sensorData.data[SensorData::gyroZ],"myGyroZ");  //x

	// Fsr Sensors
	double fsr[8];
	if(spRFsrFL != -1){
		ctrl->getSensorValue(spRFsrFL, fsr[0]);
		sensorData.data[SensorData::fsrRFL] = float(fsr[0]);
	}
	if(spRFsrFR != -1){
		ctrl->getSensorValue(spRFsrFR, fsr[1]);sensorData.data[SensorData::fsrRFR] = float(fsr[1]);
	}
	if(spRFsrBL != -1){
		ctrl->getSensorValue(spRFsrBL, fsr[2]);sensorData.data[SensorData::fsrRBL] = float(fsr[2]);
	}
	if(spRFsrBR != -1){
		ctrl->getSensorValue(spRFsrBR, fsr[3]);sensorData.data[SensorData::fsrRBR] = float(fsr[3]);
	}
	if(spLFsrFL != -1){
		ctrl->getSensorValue(spLFsrFL, fsr[4]);sensorData.data[SensorData::fsrLFL] = float(fsr[4]);
	}
	if(spLFsrFR != -1){
		ctrl->getSensorValue(spLFsrFR, fsr[5]);sensorData.data[SensorData::fsrLFR] = float(fsr[5]);
	}
	if(spLFsrBL != -1){
		ctrl->getSensorValue(spLFsrBL, fsr[6]);sensorData.data[SensorData::fsrLBL] = float(fsr[6]);
	}
	if(spLFsrBL != -1){
		ctrl->getSensorValue(spLFsrBR, fsr[7]);sensorData.data[SensorData::fsrLBR] = float(fsr[7]);
	}

	//for vision
	const int imageDelay = 33;
	int duration = SystemCall::getTimeSince(imageTimeStamp);
	if(duration >= imageDelay)
	{
		imageTimeStamp = duration >= 2 * imageDelay ? SystemCall::getCurrentSystemTime() : imageTimeStamp + imageDelay;
		frameInfo.time = imageTimeStamp;
		ctrl->getSensorValue(spCamera,imageReading);
		//saveRGBlog(imageReading);
		getImage(imageReading);
	}

	//// Time stamps for data
	sensorData.timeStamp  = SystemCall::getCurrentSystemTime();
	//MATLABHELP_PLOT(sensorData.timeStamp,"myCurrentSystemTime");	//x

	jointData.timeStamp = SystemCall::getCurrentSystemTime();

	//=======mySensorData
	mySensorData=sensorData;
	for (int i=0;i<8;i++)//for debug
	{
		mySensorData.data[SensorData::compassX]  =  mySensorData.data[SensorData::compassX]+fsr[i];
	}
	mySensorData.data[SensorData::voltage] *=  100;  //convert to % unit
	//=======end mySensorData

	//set Sensor data
	selfMessageQueue->SetMessageData(idSimSensorData,idODESimulation, idActuatorThread,
		sizeof(SensorData), (char*)(&sensorData));
	selfMessageQueue->SetMessageData(idSensoryJointData,idODESimulation, idActuatorThread,
		sizeof(JointData), (char*)(&jointData));
	//Send Robot State Data
	selfMessageQueue->SetMessageData(idRobotState,idODESimulation, idActuatorThread,
		sizeof(RobotState), (char*)(&robotState));
	//Send Robot Data to Behavior
	selfMessageQueue->SetMessageData(idSimSensorData,idODESimulation, idBehaviorThread,
		sizeof(SensorData), (char*)(&sensorData));
	selfMessageQueue->SetMessageData(idSensoryJointData,idODESimulation, idBehaviorThread,
		sizeof(JointData), (char*)(&jointData));
	//send Image Data to Vision
	selfMessageQueue->SetMessageData(idSimImage,idODESimulation,idVisionThread,
		IMAGE_WIDTH*IMAGE_HEIGHT*3, (char*)(image));
	//send Frameinfo to Vision
	selfMessageQueue->SetMessageData(idFrameInfo,idODESimulation, idVisionThread,
		sizeof(FrameInfo), (char*)(&frameInfo));
	//send to vision
	selfMessageQueue->SetMessageData(idSensoryJointData,idODESimulation, idVisionThread,
		sizeof(JointData), (char*)(&jointData));

}

void LocalRobot::setRobotRecoverd(bool recovered)
{
	robotState.fallState.recoverd = recovered;

	//set Sensor data
	selfMessageQueue->SetMessageData(idSimSensorData,idODESimulation, idActuatorThread,
		sizeof(SensorData), (char*)(&sensorData));
	selfMessageQueue->SetMessageData(idSensoryJointData,idODESimulation, idActuatorThread,
		sizeof(JointData), (char*)(&jointData));

	//Send Robot State Data
	selfMessageQueue->SetMessageData(idRobotState,idODESimulation, idActuatorThread,
		sizeof(RobotState), (char*)(&robotState));

}

void LocalRobot::setFallState(bool fall, double dx, double dy, double time)
{
	robotState.fallState.fall = fall;
	robotState.fallState.fallMetric.x = dx;
	robotState.fallState.fallMetric.y  = dy;
	robotState.fallState.fallTime = time;
}
//void LocalRobot::setRobotRecoverd(bool recovered)
//{
//	robotState.fallState.recoverd = recovered;
//
//}
const char* LocalRobot::getJointName(JointData::Joint joint)
{	
	//#define NaoV3Model	//x

	//#ifdef NaoV3Model	//x
	if(LocalRobot::naoV3)
		switch(joint)
	{
		case JointData::HeadYaw: return "HeadYaw";
		case JointData::HeadPitch: return "HeadPitch";
		case JointData::LShoulderPitch: return "LShoulderPitch";
		case JointData::LShoulderRoll: return "LShoulderRoll";
		case JointData::LElbowYaw: return "LElbowYaw";
		case JointData::LElbowRoll: return "LElbowRoll";
		case JointData::RShoulderPitch: return "RShoulderPitch";
		case JointData::RShoulderRoll: return "RShoulderRoll";
		case JointData::RElbowYaw: return "RElbowYaw";
		case JointData::RElbowRoll: return "RElbowRoll";
		case JointData::LHipYawPitch: return "LHipYawPitch";
		case JointData::LHipRoll: return "LHipRoll";
		case JointData::LHipPitch: return "LHipPitch";
		case JointData::LKneePitch: return "LKneePitch";
		case JointData::LAnklePitch: return "LAnklePitch";
		case JointData::LAnkleRoll: return "LAnkleRoll";
		case JointData::RHipYawPitch: return "RHipYawPitch";
		case JointData::RHipRoll: return "RHipRoll";
		case JointData::RHipPitch: return "RHipPitch";
		case JointData::RKneePitch: return "RKneePitch";
		case JointData::RAnklePitch: return "RAnklePitch";
		case JointData::RAnkleRoll: return "RAnkleRoll";
		default:
			return "unknown";
	}
	//#else
	else
		switch(joint)
	{
		case JointData::HeadYaw: return "headPan";
		case JointData::HeadPitch: return "headTilt";
		case JointData::LShoulderPitch: return "armLeft0";
		case JointData::LShoulderRoll: return "armLeft1";
		case JointData::LElbowYaw: return "armLeft2";
		case JointData::LElbowRoll: return "armLeft3";
		case JointData::RShoulderPitch: return "armRight0";
		case JointData::RShoulderRoll: return "armRight1";
		case JointData::RElbowYaw: return "armRight2";
		case JointData::RElbowRoll: return "armRight3";
		case JointData::LHipYawPitch: return "legLeft0";
		case JointData::LHipRoll: return "legLeft1";
		case JointData::LHipPitch: return "legLeft2";
		case JointData::LKneePitch: return "legLeft3";
		case JointData::LAnklePitch: return "legLeft4";
		case JointData::LAnkleRoll: return "legLeft5";
		case JointData::RHipYawPitch: return "legRight0";
		case JointData::RHipRoll: return "legRight1";
		case JointData::RHipPitch: return "legRight2";
		case JointData::RKneePitch: return "legRight3";
		case JointData::RAnklePitch: return "legRight4";
		case JointData::RAnkleRoll: return "legRight5";
		default:
			return "unknown";
	}
	//#endif
}

void LocalRobot::getImage(unsigned char* src)
{
	int w = IMAGE_WIDTH, h = IMAGE_HEIGHT;
	int r, g, b, yy, uu, vv;
	unsigned char y,u,v;
	for(int yh = h-1; yh >-1; --yh)
	{
		for(int xw=0;xw<w; xw++)
		{
			r = *src++;
			g = *src++;
			b = *src++;
			yy =          306 * r,
				uu = 130560 + 512 * r - 429 * g,
				vv = 130560 - 173 * r - 339 * g + 512 * b;
			yy += 601 * g + 117 * b;
			uu -= 83 * b;
			// yy =          306 * r + 601 * g + 117 * b // && 0<=r<=255, 0<=g<=255, 0<=b<=255
			// => max = 261120, min = 0
			// uu = 130048 + 512 * r - 429 * g -  83 * b // && 0<=r<=255, 0<=g<=255, 0<=b<=255
			// => max = 260 608, min = -512
			// vv = 130048 - 173 * r - 339 * g + 512 * b // && 0<=r<=255, 0<=g<=255, 0<=b<=255
			// => max = 260 608, min = -512
			// I suggest using 130560 instead of 130048 to get rid of these tests:
			//if(yy < 0) yy = 0; else if(yy > 261120) yy = 261120;
			//if(uu < 0) uu = 0; else if(uu > 261120) uu = 261120;
			//if(vv < 0) vv = 0; else if(vv > 261120) vv = 261120;
			y = (unsigned char)(yy >> 10);
			u = (unsigned char)(uu >> 10);
			v = (unsigned char)(vv >> 10);
			image[yh][xw][0]=y;
			image[yh][xw][1]=u;
			image[yh][xw][2]=v;
		}
	}
}

//void LocalRobot::saveRGBlog(unsigned char* src)
//{
//	std::ofstream ofs("rgblog.txt");
//	if (ofs.fail())
//	{
//		std::cout<<"文件打不开"<<std::endl;
//	}
//	else{
//    for (int i=0;i<320*240*3;i++)
//    {
//		ofs<<*src++;
//    }
//	}
//	ofs.close();
//
//}
//
//void LocalRobot::saveRawPic()
//{
//	std::ofstream pic("rawH1.raw",std::ofstream::binary);
//	if (pic.fail())
//	{
//		std::cout<<"cannot open rawpic"<<std::endl;
//	}
//	else
//	{
//		for (int y=0;y<IMAGE_HEIGHT;y++)
//			for (int x=0;x<IMAGE_WIDTH;x++)
//			{
//				pic<<image[y][x][0]<<image[y][x][1]<<image[y][x][2];
//			}
//	}
//	pic.close();
//}
