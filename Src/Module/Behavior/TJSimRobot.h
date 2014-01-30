#ifndef TJSimRobot_H
#define TJSimRobot_H
#include "Module/Core/Thread.h"
#include "Module/Behavior/RobotSimProvider.h"
#include "Module/Representations/BaseProvider.h"
#include "Tools/DrawDebugging/LocMessageData.h"
#include "Tools/DrawDebugging/DrawBuffer.h"
#include "Module/Common/MotionCmd.h"
#include "Simulation/SignalControl.h"

class TJSimRobot:public Thread,public BaseProvider
{
public:
	TJSimRobot(SmartPtr<Synchronizer> _synchro);
	~TJSimRobot();
	int start();
	void sendLocData();
private:
	void run();
	RobotManager robotmanager;
	LocMessageData* theLocMessageData;
};
#endif
