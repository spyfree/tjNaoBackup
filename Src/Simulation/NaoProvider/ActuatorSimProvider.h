#ifndef __ActuatorSimProvider_h_
#define __ActuatorSimProvider_h_

#include "Module/Representations/KS.h"
#include "Module/Common/JointData.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Common/SensorData.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/FrameInfo.h"
#include "Simulation/NaoProvider/SimuNaoProvider.h"
#include "boost/circular_buffer.hpp"
DEFINE_KS(ActuatorSimProvider)
REQUIRES(JointCmd)
PROVIDES(SensoryJointData)
PROVIDES(SafeJointData)
PROVIDES(MotionCmd)

PROVIDES(SensorData)
PROVIDES(SafeSensorData)
PROVIDES(ActuatorFrameInfo)
END_KS

class ActuatorSimProvider : public ActuatorSimProviderBase, public BaseProvider
{
public:
	ActuatorSimProvider();
	~ActuatorSimProvider();
	ActuatorSimProvider* getInstance(){ return ActuatorSimProvider::theInstance; }

	enum{
		FILTER_ORDER = 4
	};

	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
private:
	void update(SensoryJointData* theSensoryJointData);
	void outSensoryJointData();
	void update(MotionCmd* theMotionCmd);
	void outMotionCmd();
	void update(SafeJointData* theSafeJointData);
	void outSafeJointData();
	void update(SensorData* theSensorData);
	void outSensorData();
	void update(SafeSensorData* theSafeSensorData);
	void outSafeSensorData();
	void inJointCmd();
	void update(ActuatorFrameInfo *theActuatorFrameInfo){ theActuatorFrameInfo->time = pSensoryJointData->timeStamp; }
	void outActuatorFrameInfo(){}
	void sendJointCmd2ODESimulation(const JointCmd& jointCmd);
	void sendMotionBoardJcmdToODESimulation();
	void calculateBodyAngle(SensorData* theSensorData);
	void filter(double* Num, double* Den, const boost::circular_buffer<double>& bufData, boost::circular_buffer<double>& lastBuffer);
	//---START TRY-----//
	void startUpNao();
private:
	PROCESS_WIDE_STORAGE_STATIC ActuatorSimProvider* theInstance;
public:
	static void sendToODESimulation(const JointCmd& jointCmd);
    static void sendMotionBoardToODESimulation();
	static void finishFrame();
	static void setConnected(bool connected);
	static void setSend2Nao(bool allowed);
	static void setBodyHardness(float hardness);
	static bool connectNao(const std::string& ipadress);
	//static void setNaoProvider(MotionNaoProvider* naoProvider_);
	static void setStartUpNao();
private:
	void send();
private:
	SensoryJointData* pSensoryJointData;
	SensorData* pSensorData;
	SimuNaoProvider* naoProvider;
	std::string ipAddress;
	bool connected;
	bool send2NaoAllowed;
	bool standUp;
	JointCmd jointData;
	JointCmd jointCmd2ODESimulation;
	JointCmd jointMotionBoard;  //new added by cai
	double jd;

	//use a ring buffer later
	boost::circular_buffer<double> tiltAccBuf, rollAccBuf, lastTiltAccBuf, lastRollAccBuf;
	boost::circular_buffer<double> tiltGyroBuf, rollGyroBuf, lastTiltGyroBuf, lastRollGyroBuf;
	double lastRollGyro, lastTiltGyro;
};

#endif