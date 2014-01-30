#include "ActuatorSimProvider.h"
#include "Module/Actuator/MotionDebugging.h"

PROCESS_WIDE_STORAGE ActuatorSimProvider* ActuatorSimProvider::theInstance = NULL;

ActuatorSimProvider::ActuatorSimProvider()
:send2NaoAllowed(false),
connected(false),
standUp(false),
ipAddress("127.0.0.1"),

naoProvider(0),pSensoryJointData(0), pSensorData(0), lastRollGyro(0), lastTiltGyro(0)

{

	tiltAccBuf.set_capacity(FILTER_ORDER);
	rollAccBuf.set_capacity(FILTER_ORDER);
	tiltGyroBuf.set_capacity(FILTER_ORDER);
	rollGyroBuf.set_capacity(FILTER_ORDER);
	lastTiltAccBuf.set_capacity(FILTER_ORDER);
	lastRollAccBuf.set_capacity(FILTER_ORDER);
	lastTiltGyroBuf.set_capacity(FILTER_ORDER);
	lastRollGyroBuf.set_capacity(FILTER_ORDER);
	tiltAccBuf.clear();rollAccBuf.clear();tiltGyroBuf.clear();rollGyroBuf.clear();
	lastTiltAccBuf.clear();lastRollAccBuf.clear();lastTiltGyroBuf.clear();lastRollGyroBuf.clear();

	naoProvider = SimuNaoProvider::getInstance();
	ActuatorSimProvider::theInstance = this;
}
ActuatorSimProvider::~ActuatorSimProvider()
{
	ActuatorSimProvider::theInstance = NULL;
}

void ActuatorSimProvider::update(SensoryJointData* theSensoryJointData)
{
#ifndef ODESimulationActuator
	if(connected && naoProvider)
	{
		naoProvider->outSensorData(*theSensoryJointData);//get sensory data
		for(int i = 0; i < JointData::numOfJoints; ++i)
		{
			theSensoryJointData->angles[i] = theJointCmd->angles[i];
		}

	}
#else
		//Recieve data from ODESimulation 
		JointData temp;
		if (selfMessageQueue->SearchMyMessage(idSensoryJointData,idODESimulation, idActuatorThread,
			(char*)&temp, sizeof(JointCmd)))
		{
			for(int i = 0; i < JointData::numOfJoints; ++i)
			{
				theSensoryJointData->angles[i] = temp.angles[i];
			}
			theSensoryJointData->timeStamp = temp.timeStamp;
		}else{
			for(int i = 0; i < JointData::numOfJoints; ++i)
			{
				theSensoryJointData->angles[i] = theJointCmd->angles[i] + deg2rad(0.5) * randomDouble();
			}

		}
#endif
	// TODO: get time stamp for joints sensors
	theSensoryJointData->timeStamp = SystemCall::getCurrentSystemTime();
	MDEBUG_SAVE(MDebug::idSensoryJointData, *theSensoryJointData)
	pSensoryJointData = theSensoryJointData;


}
void ActuatorSimProvider::outSensoryJointData()
{

}
void ActuatorSimProvider::update(SafeJointData* theSafeJointData)
{
	for(int i = 0; i < JointData::numOfJoints; ++i)
		if(pSensoryJointData->angles[i] != JointData::off)
			theSafeJointData->angles[i] = pSensoryJointData->angles[i];
		else if(pSensoryJointData->angles[i] == JointData::off)
			theSafeJointData->angles[i] = 0;
	theSafeJointData->timeStamp = pSensoryJointData->timeStamp;
	selfMessageQueue->SetMessageData(idSensoryJointData,idActuatorThread, idVisionThread,
		sizeof(SafeJointData), (char*)(theSafeJointData));
}
void ActuatorSimProvider::outSafeJointData()
{

}

void ActuatorSimProvider::update(SensorData* theSensorData)
{
	//get sensor joint data from ode
	if (selfMessageQueue->SearchMyMessage(idSimSensorData,idODESimulation, idActuatorThread,
		(char*)theSensorData, sizeof(SensorData)))
	{
		calculateBodyAngle(theSensorData);
	}
	theSensorData->timeStamp = pSensoryJointData->timeStamp;
	pSensorData = theSensorData;
}
void ActuatorSimProvider::outSensorData()
{

}
void ActuatorSimProvider::update(SafeSensorData* theSafeSensorData)
{
	if(pSensorData != 0){
		for(int i = 0; i < SensorData::numOfSensors; ++i)
			theSafeSensorData->data[i] = pSensorData->data[i];

		for(int i = 0; i < JointData::numOfJoints; ++i)
		{
			theSafeSensorData->loads[i] = pSensorData->loads[i];
			theSafeSensorData->temperatures[i] = pSensorData->temperatures[i];
		}

		theSafeSensorData->timeStamp = pSensorData->timeStamp;
		MDEBUG_SAVE(MDebug::idSafeSensorData, *theSafeSensorData);
		selfMessageQueue->SetMessageData(idSimSensorData,idActuatorThread,idVisionThread,
			sizeof(SafeSensorData),(char*)(theSafeSensorData));
	}
}
void ActuatorSimProvider::outSafeSensorData()
{

}

void ActuatorSimProvider::update(MotionCmd* theMotionCmd)
{

	//First Recive data from Message Queue
	selfMessageQueue->SearchMyMessage(idMotionCmd,idMotionEngineWindow,idActuatorThread,
		(char*)theMotionCmd, sizeof(MotionCmd));

}
void ActuatorSimProvider::outMotionCmd()
{

}
void ActuatorSimProvider::inJointCmd()
{
  naoProvider->sendJoints(jointData,1000);

}
void ActuatorSimProvider::sendJointCmd2ODESimulation(const JointCmd& jointCmd)
{
	//send Joints to ODESimulation..... 
	// TODO: make better system structure
	jointCmd2ODESimulation = jointCmd;
	selfMessageQueue->SetMessageData(idJointCmd,idActuatorThread,idODESimulation, 
		sizeof(JointCmd), (char*)(&jointCmd2ODESimulation));
}

// Lowpass Chebyshev Type II, Fs = 50 Hz, Fc = 0.3 Hz
static double numLPF[ActuatorSimProvider::FILTER_ORDER] = {0.0175, -0.0171, -0.0171, 0.0175};
static double denLPF[ActuatorSimProvider::FILTER_ORDER] = {1, -2.8239, 2.6625, -0.8379}; 
static double gainLPF = 1;
static double numHPF[ActuatorSimProvider::FILTER_ORDER] = {0.9541, -2.8623, 2.8623, -0.9541};
static double denHPF[ActuatorSimProvider::FILTER_ORDER] = {1, -2.9068, 2.8165, -0.9096}; 
static double gainHPF= 1;
void ActuatorSimProvider::calculateBodyAngle(SensorData* theSensorData)
{
	//calulate current tilt and roll from Acc
#ifndef NAO_DEBUG//on real robot
	double gAcc = 59;
	double gyroOffsetX = -1942;
	double gyroOffsetY = -1474;
#else
	double gAcc = 1;
	double gyroOffsetX = 0;
	double gyroOffsetY = 0;
#endif
	theSensorData->data[SensorData::accX] *= 9.81 / gAcc;
	theSensorData->data[SensorData::accY] *= 9.81 / gAcc;
	theSensorData->data[SensorData::accZ] *= 9.81 / gAcc;
	double accX = theSensorData->data[SensorData::accX];
	double accY = theSensorData->data[SensorData::accY];
	double accZ = theSensorData->data[SensorData::accZ];
	double bodyTilt = atan2(accX, sqrt(accY * accY + accZ* accZ));
	double bodyRoll = atan2(-accY, sqrt(accX * accX + accZ * accZ));
	tiltAccBuf.push_front(bodyTilt);
	rollAccBuf.push_front(bodyRoll);
	double dt = 0.02; //s, time per frame, please use time stamp later, more flexible
	double gyroX = theSensorData->data[SensorData::gyroX];
	double gyroY = theSensorData->data[SensorData::gyroY];
#ifndef NAO_DEBUG//on real robot
	double rollGyro = deg2rad((gyroX - gyroOffsetX) * dt / 2.7)+ lastRollGyro;
	double tiltGyro =  deg2rad((gyroY - gyroOffsetY) * dt / 2.7)+ lastTiltGyro;
#else
	double rollGyro = (gyroX - gyroOffsetX) * dt + lastRollGyro;
	double tiltGyro =  (gyroY - gyroOffsetY) * dt + lastTiltGyro;
#endif
	lastRollGyro = rollGyro;
	lastTiltGyro = tiltGyro;
	rollGyroBuf.push_front(rollGyro);
	tiltGyroBuf.push_front(tiltGyro);
	filter(numLPF, denLPF, rollAccBuf, lastRollAccBuf);
	filter(numLPF, denLPF, tiltAccBuf, lastTiltAccBuf);
	filter(numHPF, denHPF, rollGyroBuf, lastRollGyroBuf);
	filter(numHPF, denHPF, tiltGyroBuf, lastTiltGyroBuf);
	double angleX = gainLPF * lastRollAccBuf.front() + gainHPF * lastRollGyroBuf.front();
	double angleY = gainLPF * lastTiltAccBuf.front() + gainHPF * lastTiltGyroBuf.front();
	theSensorData->data[SensorData::angleX] = angleX;
	theSensorData->data[SensorData::angleY] = angleY;

}
void ActuatorSimProvider::filter(double* Num, double* Den, const boost::circular_buffer<double>& bufData, boost::circular_buffer<double>& lastBuffer)
{
	double temp=0;
	for(int i = 0; i < FILTER_ORDER && i < bufData.size();++i)
	{
		temp += Num[i] * bufData[i];
	}
	for(int i = 1; i < FILTER_ORDER && i < lastBuffer.size(); ++i)
	{
		temp -= Den[i] * lastBuffer[i-1];
	}
	lastBuffer.push_front(temp);
}

void ActuatorSimProvider::startUpNao()
{
	//inJointCmd();
}
void ActuatorSimProvider::send()
{
	inJointCmd();
}
//newly added for send SimuMotionBoard_jointAngle to ODESimulation
void ActuatorSimProvider::sendMotionBoardJcmdToODESimulation()
{
	double jdataDeg[] = {0, 0,
		0,0,0,0,//100, -30, 90, 90,
		0,0,0,0,//100, 30, -90, -90,
		0, 0, -40, 80, -40, 0,
		0, 0, -40, 80, -40, 0};
	
	for (int i = JointData::NoneJoint + 1; i < JointData::numOfJoints; ++i)
	{
		jointMotionBoard.angles[i] = deg2rad(jdataDeg[i-1]);	
	}

	/*selfMessageQueue->SetMessageData(idMotionBoardJointCmd,idActuatorThread,idODESimulation,
		(char*)(&jointMotionBoard),sizeof(JointCmd));*/
}
void ActuatorSimProvider::sendMotionBoardToODESimulation()
{
	if (theInstance)
	     theInstance->sendMotionBoardJcmdToODESimulation();
}
void ActuatorSimProvider::sendToODESimulation(const JointCmd& jointCmd)
{
	if(theInstance)
		theInstance->sendJointCmd2ODESimulation(jointCmd);
}
void ActuatorSimProvider::finishFrame()
{
	if(theInstance)
		theInstance->send();
}
void ActuatorSimProvider::setConnected(bool connected)
{
	if(theInstance)
		theInstance->connected = connected;
}
void ActuatorSimProvider::setSend2Nao(bool allowed)
{
	if(theInstance)
		theInstance->send2NaoAllowed = allowed;
}
void ActuatorSimProvider::setBodyHardness(float hardness)
{
	if(theInstance && theInstance->naoProvider)
	{
		theInstance->naoProvider->setBodyHardness(hardness);
	}
}
bool ActuatorSimProvider::connectNao(const std::string& ipadress)
{
	if(theInstance)
	{
		return (theInstance->naoProvider && 
			theInstance->naoProvider->connectNao(ipadress));
	}
}
// void ActuatorSimProvider::setNaoProvider(MotionNaoProvider* naoProvider_)
// {
// 	if(theInstance)
// 		theInstance->naoProvider = naoProvider_;
// }
void ActuatorSimProvider::setStartUpNao()
{
	if(theInstance)
		theInstance->startUpNao();
}

MAKE_KS(ActuatorSimProvider)